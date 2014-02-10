#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
//    lastPolyt = ofGetElapsedTimef();
//    polytime = 1;
	globalHeight=240;
    globalWidth=320;
#ifdef LOCALOSC    
    ofSetFrameRate(20);
#else
    ofSetFrameRate(20);
#endif


    #ifdef _USE_LIVE_VIDEO

    vidGrabber.setVerbose(true);
       #ifdef MAC_OS_X_VERSION_10_7   
    vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    vidGrabber.setGrabber(vidRecorder);
#endif
    vidGrabber.initGrabber(globalWidth,globalHeight);
           #ifdef MAC_OS_X_VERSION_10_7
    vidRecorder->setDeviceID(1);
#endif
    vidGrabber.setDesiredFrameRate(30);
    

   	#else
//    ofImage loader;
//    loader.loadImage("shadow3.jpeg");
//    colorImg.setFromPixels(loader.getPixels(),loader.width, loader.height);
//    colorImg.resize(globalWidth, globalHeight);
    
   vidPlayer.loadMovie("bien.mov");
   vidPlayer.play();
	#endif
   const int N = 4;
    
   
    grayThresImg.allocate(globalWidth,globalHeight);
    colorImg.allocate(globalWidth,globalHeight);
    
	threshold = 44;
    diff_threshold = 30;
    diff_mhi =99;
    
    thresDist = 220.9f;
    
    //simplify  = 11.5f;
    simplify  = 7.0f;
#ifdef DEBUG_EXTREM
    currPoint = 0;
#endif
    
    resample = 51;
    
    //BlobMin
    blobMin = 2000;
    
    // Trapeze correction
    editTrapeze = false;
    clearPoint();
    
    // Find extrem
    sureLimit = -2.1;
    doubtLimit = -0.6;

    
    // Send Receive message
#ifdef LOCALOSC
    clientDestination = "localhost";
#else
    clientDestination = "192.168.43.100";
#endif
	clientSendPort = 12345;
	clientSender.setup(clientDestination, clientSendPort);
    isSending = true;
    
    
    clientReceiver.setup(12344);
    
}

//--------------------------------------------------------------
void testApp::update(){
    

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else 
        bNewFrame=true;
        vidPlayer.update();
	#endif


	if (bNewFrame)
    {

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(),globalWidth,globalHeight);
        #else
           colorImg.setFromPixels(vidPlayer.getPixels(), globalWidth,globalHeight);
        #endif
        
        grayThresImg=colorImg;
        grayThresImg.threshold(threshold,true);
        grayThresImg.blur(3);
        
        
        #ifdef _USE_LIVE_VIDEO
        grayThresImg.invert();
        #endif
        
        //Trapeze correction
        grayThresImg.warpPerspective(ptE,ptF,ptC,ptD);
        
        // Set the size of the minimal blob
        receiveMessage();
        
        // Blob detection
        contourFinder.findContours(grayThresImg, blobMin, (globalHeight*globalWidth)/2, 10, false,false);	// find holes
            
            // Blob is found
            if (contourFinder.blobs.size() > 0 )
            {
                bigBlob = contourFinder.blobs.front();
                
                //Find hands and feets
                findExtrem(bigBlob, simplify);
                
                // set attractors according to the postion
                recognizeGesture();
                
                // send attractors
                sendAttractorMessage();
            
            }
            // Blob is not found
            else
            {
                blobLine.clear();
                correctBlobLine.clear();
                sendPolylineMessage(false);
                
                forceCount = 0;
                family1 = ofPoint(0,0);
                family2 = ofPoint(0,0);
                family2_2 = ofPoint(0,0);
                family3 = ofPoint(0,0);
                sendAttractorMessage();
                
            }
        
        }
    

}

//--------------------------------------------------------------
void testApp::draw(){

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
    ofFill();
    ofTranslate(20,20);
    colorImg.draw(0,0);
	grayThresImg.draw(0,260);
    
    
    //Draw the point of trapeze correction
    
    if(editTrapeze)
    ofSetColor(210, 210, 180);
    else
    ofSetColor(200, 100, 160);
    
    if (editTrapeze && pointEdit == 0)
        ofCircle(ptE,10);
    else ofCircle(ptE,5);
    if (editTrapeze && pointEdit == 1)
        ofCircle(ptF,10);
    else ofCircle(ptF,5);
    if (editTrapeze && pointEdit == 2)
        ofCircle(ptD,10);
    else ofCircle(ptD,5);
    if (editTrapeze && pointEdit == 3)
        ofCircle(ptC,10);
    else ofCircle(ptC,5);

    if( editTrapeze){
    ofSetColor(255, 0, 0);
    ofCircle(ptTest, 7);
    }
    
    ofSetColor(0);
    
    // Draw the original blob 

    ofTranslate(380,0);

    blobLine.draw();
    for(int k=0;k<extrem.size();k++)
    {
            ofCircle(extrem[k],3);
    }
    ofSetColor(30,20,120);
    ofTranslate(0,260);
    
#ifdef DEBUG_EXTREM
    ofCircle(currPointToDraw, 3);
    ofSetColor(0);
#endif
    
    //Draw Corrected blob with the attactors
    
    correctBlobLine.draw();
    int extremSize = extrem.size();
    
    ofSetColor(255, 0,0);
    if(family1.x*family1.y >0)
        ofCircle(family1, 10);
    ofSetColor(0, 255,0);
    if(family2.x*family2.y >0)
        ofCircle(family2, 10);
    if(family2_2.x*family2_2.y >0)
        ofCircle(family2_2,10);
    ofSetColor(0, 0,255);
    if(family3.x*family3.y >0)
        ofCircle(family3, 10);
    

	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
#ifdef DEBUG_EXTREM
	sprintf(reportStr, "threshold:%d, \nCurrent Angle :%f, \n NextVect Lenght:%f, \nSimplify:%f", threshold, currAngle, currNextLength, simplify);
#else
    sprintf(reportStr, "threshold:%d, \nBlobMin:%d, \nSimplify:%f \nSureLimit :%f \nDoubtLimit : %f", threshold, blobMin, simplify, sureLimit, doubtLimit);
#endif
	ofDrawBitmapString(reportStr, 280, 60);
    ofColor(255,255,255);
    
    
    
    
}
//--------------------------------------------------------------
void testApp::findExtrem(ofxCvBlob blob, float simpl){
 
    
    
    blobLine.clear();
    correctBlobLine.clear();
    extrem.clear();
    
    // Convert ofxCvBlob into ofPolyline 
    for( int i=0; i<blob.nPts;i++ )
    {
        ofPoint point = blob.pts.at(i);
        blobLine.lineTo(point.x,point.y);
        
    }
    
    //Simplify 1st time to send to video app
    blobLine = blobLine.getResampledByCount(resample);
    //blobLine.simplify(1.8);
    
    // Trapeze correction after simplify function
    for(int i=0; i<blobLine.size(); i++)
    {
        ofPoint original = blobLine[i];
        correctBlobLine.lineTo(original);
    }
    
    sendPolylineMessage( true);
    
    //Simplify 2nd to calculate the extrem points
    correctBlobLine.simplify(simpl);
    
    //Read point one by one
    int i=0;
    int nbPoint=correctBlobLine.getVertices().size();
    float space= 200.0/nbPoint;
    

    while(i<nbPoint)
    {
        
        ofPoint point=correctBlobLine[i];
        ofPoint prevPoint, nextPoint, nextnextPoint;
        if(i==0) prevPoint=correctBlobLine[nbPoint-1];
        else prevPoint=correctBlobLine[i-1];
        if(i==(nbPoint-1)){
            nextPoint=correctBlobLine[0];
            nextnextPoint=correctBlobLine[1];
        }
        else nextPoint=correctBlobLine[i+1];
        if(i==nbPoint-2)
            nextnextPoint=correctBlobLine[1];
        if(i<nbPoint-2)
            nextnextPoint=correctBlobLine[i+2];

        
        ofVec2f curVect = ofVec2f((point.x-prevPoint.x), (point.y-prevPoint.y));
        ofVec2f nextVect = ofVec2f((nextPoint.x-point.x), (nextPoint.y-point.y));
        ofVec2f nextnextVect = ofVec2f((nextnextPoint.x-nextPoint.x), (nextnextPoint.y-nextPoint.y));
        
        ofPoint centre=blob.centroid;
        ofPoint extremeFound;
        bool isConvex=false;
        
        
        //TODO mettre ces valeurs controllables dynamiquement

        
        float convexAngle;
        
        // Calculate Angle
        convexAngle = calcAngle(curVect, nextVect );
        
        
        // Current Point to draw : debug addon
#ifdef DEBUG_EXTREM
        if(i==currPoint){
            currAngle=convexAngle;
            currPointToDraw=point;
            currNextLength=nextVect.length();
        }
#endif
        
        // RULES to find an extrem point
        if (convexAngle< sureLimit)
        {
            isConvex=true;
            extremeFound = point;
        }
        
        if( convexAngle < doubtLimit && convexAngle> sureLimit  && nextVect.length()<45 )
        {
            float nextConvexAngle = calcAngle(nextVect, nextnextVect);
            if(  (nextConvexAngle+convexAngle ) < sureLimit && nextConvexAngle>sureLimit)
            {
            
            ofPoint midlePoint;
            midlePoint.x= int((point.x + nextPoint.x) / 2);
            midlePoint.y=int ((point.y + nextPoint.y)/2);
            extremeFound=midlePoint;
            isConvex=true;
                
           
            }
                
            
        }
        
        // Extreme is found, add it to Polyline
        if(isConvex)
        {
            extrem.push_back(extremeFound);
        }
        
        i++;
        
        
    }
    
    
    
}
//--------------------------------------------------------------
float testApp::calcAngle(ofVec2f vec1, ofVec2f vec2){
    
    float res;
    ofVec2f vec1n = vec1.getNormalized();
    ofVec2f vec2n = vec2.getNormalized();
    
    float cos = vec1n.dot(vec2n);
    float angle = acos(cos);
    
    if(vec1.x * vec2.y - vec1.y * vec2.x >0)
        res=angle;
    else
        res=-angle;


    return res;

}

//--------------------------------------------------------------
int testApp::sendPolylineMessage(bool isBlob)
{
    
    
    
    
    if(isSending)
    {
    
        // Send Polyline
        ofxOscMessage mes;
        int nbPoints;
        
        mes.setAddress("/polyline");
        
        if(isBlob){
                nbPoints= correctBlobLine.getVertices().size();
        }
        else{
            nbPoints = 0;
        }
        
        
        mes.addIntArg(nbPoints);
        
        if( isBlob)
        {
            for(int k=0;k<nbPoints;k++)
            {
                mes.addFloatArg((correctBlobLine[k]).x*1.0/globalWidth);
                mes.addFloatArg((correctBlobLine[k]).y*1.0/globalHeight);
            }
            
        }
        
        clientSender.sendMessage(mes);
        
    }

    return 1;
    
}

//--------------------------------------------------------------
int testApp::sendAttractorMessage()
{
    
    if( isSending)
    {
    
        // Send Polyline
        ofxOscMessage mes;

        mes.setAddress("/attractors");
        mes.addIntArg(forceCount);
        
        if(family1.x*family1.y >0){
        mes.addIntArg(0);
        mes.addFloatArg(1.0f-(family1.x*1.0)/globalWidth);
        mes.addFloatArg((family1.y*1.0)/globalHeight);
        }

        if(family2.x*family2.y >0){
        mes.addIntArg(1);
        mes.addFloatArg(1.0f-(family2.x*1.0)/globalWidth);
        mes.addFloatArg((family2.y*1.0)/globalHeight);
        }

        if(family2_2.x*family2_2.y >0){
        mes.addIntArg(1);
        mes.addFloatArg(1.0f-(family2_2.x*1.0)/globalWidth);
        mes.addFloatArg((family2_2.y*1.0)/globalHeight);
        }

        if(family3.x*family3.y >0){
        mes.addIntArg(2);
        mes.addFloatArg(1.0f-(family3.x)/globalWidth);
        mes.addFloatArg((family3.y*1.0)/globalHeight);
        }

        
        clientSender.sendMessage(mes);
        
    }
    
    return 1;
    
}

//--------------------------------------------------------------
int testApp::recognizeGesture()
{

    forceCount=0;
    ofRectangle rect = correctBlobLine.getBoundingBox();
    ofPoint     centre  = correctBlobLine.getCentroid2D();


    // Family 1
    //if(rect.height*1.0f > (globalHeight*0.6f))
    if(centre.y*1.0 <= globalHeight*0.7)

    {
        family1=centre;
        forceCount++;
        
    }
    else family1=ofPoint(0,0);

    // Family 2
    int index=-1;
    float distanceMax=0;
    int index2=-1;
    float distanceMax2 = 0;
    float distance;
    float alpha, alpha2;


    // Condition of the rectangle to apply familly 2 attractor
     if(
        //rect.width*1.0f > rect.height*0.5
        rect.height*1.0f > (globalHeight*0.65f)
        )
    {
    
        for(int k=0;k<extrem.size();k++)
        {

            distance= (float)abs(centre.x - extrem[k].x);
            
            
            // FIRST ATTRACTOR ( left hand / right hand )
            if( distance>distanceMax
               && extrem[k].y < (globalHeight-40)
               && index==-1
               && abs(centre.x - extrem[k].x)> 20                           //Avoid head
               )
            {
                  index=k;
                  distanceMax=distance;
            }
            
            // SECOND ATTRACTOR ( other hand )
            if(  extrem[k].y < (globalHeight-40)                            // Avoid foot
               &&((centre.x - extrem[index].x)*(centre.x - extrem[k].x))<0  // Avoid attractor on the same side
               && index2==-1                                                // Avoid recopy
               && abs(centre.x - extrem[k].x)> 20)                          // Avoid head
            {
                index2=k;
                distanceMax2=distance;
            }
            
        }
        
        if(abs(family2.distance( extrem[index]) > 25)) alpha=0.0f;
        else{
           alpha=0.8f;
        }

        float new2x =  ((family2.x*1.0f * alpha) + extrem[index].x*1.0f*(1.0f - alpha));
        float new2y =  ((family2.y*1.0f * alpha) + extrem[index].y*1.0f*(1.0f - alpha));
        family2 = ofPoint( static_cast<int>(new2x),static_cast<int>( new2y));
            forceCount++;
        
        // If the second one is really close from the first one AND on the same side
        if(index2  != -1)
        {
        
            if(abs(family2_2.distance( extrem[index2]) > 25)) alpha2=0.0f;
            else alpha2=0.8f;
        
            new2x =  ((family2_2.x*1.0f * alpha2) + extrem[index2].x*1.0f*(1 - alpha2));
            new2y =  ((family2_2.y*1.0f * alpha2) + extrem[index2].y*1.0f*(1 - alpha2));
            family2_2 = ofPoint( static_cast<int>(new2x),static_cast<int>( new2y));
            
            forceCount++;
        }
        else family2_2=ofPoint(0,0);
           
    }
    else
    {
    family2=ofPoint(0,0);
    family2_2 = ofPoint(0,0);
    }
    
    //Family 3
    //if(rect.height*1.0 < globalHeight*0.6)
    if(centre.y*1.0 > globalHeight*0.7)
    {
        
        family3=centre;
        forceCount++;
        
    }
    else family3 = ofPoint(0,0);
            
        

    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case '+':
			threshold ++;
			if (diff_threshold > 255) diff_threshold = 255;
			break;
		case '-':
			threshold --;
			if (diff_threshold < 0) diff_threshold = 0;
			break;
#ifdef DEBUG_EXTREM
        case 'a':
			currPoint--;
			break;
        case 'z':
			currPoint++;
            break;
#endif
        case 'w':
			simplify-=0.5;
			break;
        case 'x':
			simplify+=0.5;
			break;
        case 'e':
			if(!editTrapeze){
                clearPoint();
                editTrapeze=true;
            }
            else
                editTrapeze=false;
			break;
        case 'y':
            translateTrapeze(false, false);
            break;
        case 'h':
            translateTrapeze(true, false);
            break;
        case 'g':
            translateTrapeze(true, true);
            break;
        case 'j':
            translateTrapeze(false, true);
            break;
        case 'q':
            resample--;
            break;
        case 's':
            resample++;
            break;
        case 'o':
            sureLimit -= 0.1;
            break;
        case 'p':
            sureLimit += 0.1;
            break;
        case 'l':
            doubtLimit -= 0.1;
            break;
        case 'm':
            doubtLimit += 0.1;
            break;
#ifndef MAC_OS_X_VERSION_10_7
        case 'n':
          //  vidGrabber.videoSettings();
            break;
#endif
            
        case' ':
        #ifdef _USE_LIVE_VIDEO
        
        #else
        vidPlayer.setPaused(!vidPlayer.isPaused());
        #endif
            break;
	}
}

//--------------------------------------------------------------
void testApp::clearPoint(){
    
    //1-135 D
    //28-63 E
    // 264 - 63 F
    // 291 - 235 C
    
    
    ptE = ofPoint(28,63);
    ptF = ofPoint(264,63);
    ptC = ofPoint(291,235);
    ptD = ofPoint(1, 235);
    ptTest = ofPoint(0, 160);
    pointEdit = 0;
    
}

//--------------------------------------------------------------
ofPoint testApp::correctTrapeze(ofPoint original){
    
    ofPoint ptA = ofPoint(ptD.x, ptE.y);
    ofPoint ptB = ofPoint(ptC.x, ptE.y);
    

    
    int x = original.x;
    int y = original.y;
    
    float x_, x__;
    float y_;
    
    int dAB= ptB.x-ptA.x;
    int dEF = ptF.x-ptE.x;
    int dAD = ptD.y - ptA.y;
    
    x_ = (float) (x - ptA.x)*1.0f*globalWidth/(dAB*1.0f);
    y_ = (float) (y - ptA.y)*1.0f*globalHeight/(dAD*1.0f);
    
    if(x_ < 0) x_ = 0;
    if(x_ > 320) x_ = 320;
    if(y_ < 0) y_ = 0;
    if(y_ > 240) y_ = 240;
    
    float off = (float)((globalHeight - y_ )/(globalHeight*1.0f) )*1.0*(ptE.x - ptA.x)*(globalWidth*1.0f)/(dAB*1.0f);
    
    x__ = (float) (x_ - off )*1.0f*(globalWidth*1.0f/ (globalWidth*1.0f - (off*2.0) ));
    if(x__ < 0) x__ = 0;
    if(x__ > 320) x__ = 320;
    
    return ofPoint(x__, (int)y_);

    
}

//--------------------------------------------------------------
void testApp::translateTrapeze(bool isIscreasing, bool isHorizontal){
    
    if(isIscreasing){
        
        if(isHorizontal)
        {
            ptE = ofPoint(ptE.x+1, ptE.y);
            ptF = ofPoint(ptF.x+1, ptF.y);
            ptD = ofPoint(ptD.x+1, ptD.y);
            ptC = ofPoint(ptC.x+1, ptC.y);
        }
        else
        {
            ptE = ofPoint(ptE.x, ptE.y+1);
            ptF = ofPoint(ptF.x, ptF.y+1);
            ptD = ofPoint(ptD.x, ptD.y+1);
            ptC = ofPoint(ptC.x, ptC.y+1);
        }
        
    }
    else
    {
        if(isHorizontal)
        {
            ptE = ofPoint(ptE.x-1, ptE.y);
            ptF = ofPoint(ptF.x-1, ptF.y);
            ptD = ofPoint(ptD.x-1, ptD.y);
            ptC = ofPoint(ptC.x-1, ptC.y);
        }
        else
        {
            ptE = ofPoint(ptE.x, ptE.y-1);
            ptF = ofPoint(ptF.x, ptF.y-1);
            ptD = ofPoint(ptD.x, ptD.y-1);
            ptC = ofPoint(ptC.x, ptC.y-1);
        }
    
    
    }



}




//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    // Trapeze Editing
    if(editTrapeze)
    {
        
        if(ofGetMouseX() >20 && ofGetMouseX()<340 && ofGetMouseY() >20 && ofGetMouseY() < 260)
        {
            
            switch (pointEdit){
                case 0:
                    ptE = ofPoint(ofGetMouseX()-20, ofGetMouseY()-20);
                    break;
                case 1:
                    ptF = ofPoint(ofGetMouseX()-20, ofGetMouseY()-20);
                    if (ptF.x< ptE.x) ptF.x = ptE.x;
                    break;
                case 2:
                    ptD = ofPoint(ofGetMouseX()-20, ofGetMouseY()-20);
                    if(ptD.y < ptE.y)ptD.y = ptE.y;
                    if(ptD.y < ptF.y)ptD.y = ptF.y;
                case 3:
                    ptC = ofPoint(ofGetMouseX()-20, ofGetMouseY()-20);
                    if (ptC.x< ptD.x) ptC.x = ptD.x;
                    if(ptC.y < ptE.y)ptC.y = ptE.y;
                    if(ptC.y < ptF.y)ptC.y = ptF.y;
                    break;
                    
            }
            
            pointEdit++;
                
        }
        
        // Test point = show trapeze correction result
        if(pointEdit > 2 && ofGetMouseX() >20 && ofGetMouseX()<340 && ofGetMouseY() >20 && ofGetMouseY() < 260)
        {
            
            ofPoint original = ofPoint(ofGetMouseX()-20, ofGetMouseY()-20);
            ptTest = correctTrapeze(original);
            
        }
        
        
    }
    
}

void testApp::receiveMessage(){
    
    
    ofxOscMessage msg = ofxOscMessage();
    clientReceiver.getNextMessage(&msg);
    
    
    // Set Blob min Size
    if(msg.getAddress()== "/BlobMin"){
        
        int size = msg.getArgAsInt32(0);
        
        if(size>0) blobMin = size;
        
    }
    
    if(msg.getAddress()== "/OSCSend"){
        
        isSending = msg.getArgAsInt32(0)==1;
        
    }
    
    // Send OSC sending
    if(msg.getAddress()== "/BlobThresh"){
        if(msg.getArgAsInt32(0)>0) threshold = msg.getArgAsInt32(0);
               
    }
    
    
    
    
    
    
    
}


