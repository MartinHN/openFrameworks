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
     //  #ifdef MAC_OS_X_VERSION_10_7   
    //vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    //vidGrabber.setGrabber(vidRecorder);
    
    kinect.setRegistration(true);
	kinect.init();
    kinect.open();
    

    

//#endif
    //vidGrabber.initGrabber(globalWidth,globalHeight);
#ifdef MAC_OS_X_VERSION_10_7
    //vidRecorder->setDeviceID(1);
#endif
    //vidGrabber.setDesiredFrameRate(30);
    

   	#else
//    ofImage loader;
//    loader.loadImage("shadow3.jpeg");
//    colorImg.setFromPixels(loader.getPixels(),loader.width, loader.height);
//    colorImg.resize(globalWidth, globalHeight);
    
//   vidPlayer.loadMovie("bien.mov");
//   vidPlayer.play();
	#endif
    const int N = 4;
    
    useKinect = true;
    

   
    grayKinect.allocate(kinect.width/2, kinect.height/2);
    grayThresImg.allocate(globalWidth,globalHeight);
    colorImg.allocate(kinect.width,kinect.height);
    
    grayThreshNear.allocate(globalWidth, globalHeight);
	grayThreshFar.allocate(globalWidth, globalHeight);
    
    nearThreshold = 255;
	farThreshold = 219;
    
	threshold = 44;
    diff_threshold = 30;
    
    thresDist = 220.9f;
    
    
    
    kinectWarp.setSrc(ofPoint(0,0),
                      ofPoint(grayKinect.getWidth(),0),
                      ofPoint(grayKinect.getWidth(),grayKinect.getHeight()),
                      ofPoint(0,grayKinect.getHeight()));
    kinectWarp.setDst(ofPoint(0,0),
                      ofPoint(300,0),
                      ofPoint(120,300),
                      ofPoint(2, 30));
    dragWarpPt=-1;
    
    //simplify  = 11.5f;
    simplify  = 6.5f;
    
    family_limit = 0.695;
#ifdef DEBUG_EXTREM
    currPoint = 0;
#endif
    
    resample = 51;
    
    //BlobMin
    blobMin = 1200;
    
    // Trapeze correction
    editTrapeze = false;
    clearPoint();
    


    
    // Send Receive message
#ifdef LOCALOSC
    clientDestination = "127.0.0.1";
#else
    clientDestination = "100.200.100.200";
#endif
	clientSendPort = 12345;
	clientSender.setup(clientDestination, clientSendPort);
    isSending = true;
    
    clientReceiver.setup(12344);
    ofAddListener(clientReceiver.onMessageReceived, this, &testApp::receiveMessage);

    
}

//--------------------------------------------------------------
void testApp::update(){
    

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
  #if OF_VERSION >7  
    if(!useKinect){
        kinect.update();
        bNewFrame = kinect.isFrameNewVideo();

    }
    else{

    kinect.update();
        bNewFrame = kinect.isFrameNewDepth();
    }
#else
    kinect.update();
    bNewFrame = kinect.isFrameNew();
#endif
    
    #else 
        bNewFrame=true;
        vidPlayer.update();
	#endif


	if (bNewFrame)
    {

		#ifdef _USE_LIVE_VIDEO
        
        if(useKinect){
        
        grayKinect.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        grayKinect.resize(globalWidth, globalHeight);
        
        grayThresImg = grayKinect;
        
        grayThreshNear = grayThresImg;
        grayThreshFar = grayThresImg;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayThresImg.getCvImage(), NULL);
            
        
        }
        else{
        
        colorImg.setFromPixels(kinect.getPixels(),kinect.width,kinect.height);
        colorImg.resize(globalWidth, globalHeight);
        grayThresImg = colorImg;
        grayThresImg.threshold(threshold);
            
        }

        
        #else
        colorImg.setFromPixels(vidPlayer.getPixels(), globalWidth,globalHeight);
        grayThresImg=colorImg;
        grayThresImg.threshold(threshold,true);
        #endif
        
 
        
        
        
        #ifdef _USE_LIVE_VIDEO

        //grayThresImg.invert();
        
        
        
        #endif
        
        //Trapeze correction
        grayThresImg.warpPerspective(ptE,ptF,ptC,ptD);
        

        
        // Set the size of the minimal blob AND the camera type
       // receiveMessage();
        
        //secu blob min
        if( !useKinect) blobMin = 1;
        
        // Blob detection
        contourFinder.findContours(grayThresImg, blobMin, (globalHeight*globalWidth)/2, 10, false,false);	// find holes
            
            // Blob is found
            if (contourFinder.blobs.size() > 0 )
            {
                bigBlob = contourFinder.blobs.front();
                
                //Find hands and feets and send polyline
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
    if(!isFullScreen){
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
    ofFill();
    ofTranslate(20,20);
    
    if(useKinect)
    grayKinect.draw(0,0);
    else
        colorImg.draw(0, 0);
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
    
#ifdef DEBUG_EXTREM
    ofCircle(currPointToDraw, 3);
    ofSetColor(0);
#endif
    
    
    ofSetColor(100);
    
    //Rectangle of the stage
    ofRect(0, 0, 320, 240);
    
    // Rectangle of kinect field of view
    float warpedWidth = static_cast<float>(globalWidth*1.0f*unzoomedSize);
    float warpedHeight = static_cast<float>(globalHeight*1.0f*unzoomedSize);
    ofSetColor(180);
    ofRect(unzoomedCentre.x - warpedWidth/2.0f, unzoomedCentre.y - warpedHeight/2.0f, warpedWidth, warpedHeight);
    //ofRect(unzoomedCentre.y - warpedWidth/2.0f, unzoomedCentre.y - warpedHeight/2.0f, warpedWidth, warpedHeight);
    
    //Center of the fielf of view
    ofSetColor(255);
    ofCircle(unzoomedCentre, 5);
    ofSetColor(0);
    
    
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
    
    }
    else{
//        grayKinect.updateTexture();

     
        kinectWarp.drawControlPoints();
        kinectWarp.loadMat();
        grayThresImg.draw(0,0);
        kinectWarp.unloadMat();
       
        
        //warpPerspective( kinectMapPts[0],kinectMapPts[1],kinectMapPts[2],kinectMapPts[3] );
       // tstImg.updateTexture();

        //tstImg.draw(0,0);
        
        
    }

	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
#ifdef DEBUG_EXTREM
	sprintf(reportStr, "threshold:%d, \nCurrent Angle :%f, \n NextVect Lenght:%f, \nSimplify:%f", threshold, currAngle, currNextLength, simplify);
#else
    
    if(editTrapeze){
    sprintf(reportStr, "pt E:%f, %f, \npt F:%f, %f , \npt D: %f, %f  \nptC:%f, %f ", ptE.x, ptE.y, ptF.x, ptF.y, ptD.x, ptD.y, ptC.x,ptC.y);
    }
    else {
    sprintf(reportStr, "threshold Near:%d, thresholdFar:%d , \nfamiLimit:%F \nBlobMin:%d, \nSimplify:%f  ", nearThreshold, farThreshold, family_limit , blobMin, simplify);
    }
#endif



    
    
    ofDrawBitmapString(reportStr, 0, 300);
    
    
    if(useKinect)
        ofDrawBitmapStringHighlight( " Depth camera", ofPoint(-370, 0), ofColor(255, 0, 0), ofColor(0, 0, 0));
    else
        ofDrawBitmapStringHighlight( " RGB camera", ofPoint(-370, 0), ofColor(255, 0, 0), ofColor(0, 0, 0));
    
    
    
    ofDrawBitmapStringHighlight( "BlobMin: "+ofToString(blobMin) , ofPoint(-230, 0), ofColor(255, 0, 0), ofColor(0, 0, 0));
    
    
    if(isSending)
        ofDrawBitmapStringHighlight( " IS Sending", ofPoint(-100, 0), ofColor(0, 255, 0), ofColor(0, 0, 0));
    else
        ofDrawBitmapStringHighlight( " is NOT Sending", ofPoint(-100, 0), ofColor(255, 0, 0), ofColor(0, 0, 0));
    
    
   
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
    //blobLine = blobLine.getR
    
    
    
    // Trapeze correction after simplify function
    for(int i=0; i<blobLine.size(); i++)
    {
        ofPoint original = blobLine[i];
        correctBlobLine.lineTo(unzoomedPolyline(original));
    }
    
    sendPolylineMessage( true);
    
    
    //Simplify 2nd to calculate the extrem points
    //correctBlobLine.simplify(simpl);
    

    
    
    
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

//-------------------------------------------------------------
ofPoint  testApp::unzoomedPolyline(ofPoint p)
{
    
    float originalX = p.x;
    float finalX = p.x*unzoomedSize + unzoomedCentre.x - (globalWidth/2.0f)*unzoomedSize;
    
    float originalY = p.y;
    float finalY = p.y*unzoomedSize + unzoomedCentre.y - (globalHeight/2.0f)*unzoomedSize;
    
    return ofPoint(finalX, finalY);
    
    
    
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
   
    ofPoint     centre  =  correctBlobLine.size()>5?correctBlobLine.getCentroid2D():ofPoint(rect.x,rect.y);
    

    // Family 1
    //if(rect.height*1.0f > (globalHeight*0.6f))
    if(centre.y*1.0 <= globalHeight*family_limit)

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
        //rect.height*1.0f > (globalHeight*0.65f)
        false
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
    if(centre.y*1.0 > globalHeight*family_limit)
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
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
        case 'c':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
        case 'v':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
        case 'd':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
        case 'f':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
            
            
            case 'i':
            isFullScreen = !isFullScreen;
            ofSetFullscreen(isFullScreen);
            break;
            
#ifdef DEBUG_EXTREM
        case 'a':
			currPoint--;
			break;
        case 'z':
			currPoint++;
            break;
#else
        case 'a':
			simplify-=0.1;
			break;
        case 'z':
			simplify+=0.1;
            break;
        case 'A':
			unzoomedSize = unzoomedSize - 0.05;
            if(unzoomedSize < 0.05 ) unzoomedSize = 0.05;
			break;
        case 'Z':
			unzoomedSize = unzoomedSize + 0.05;
            if(unzoomedSize > 1.0f) unzoomedSize = 1.0f;
            break;

#endif
//        case 'w':
//			family_limit-=0.01;
//			break;
//        case 'x':
//			family_limit+=0.01;
//			break;
        case 'e':
			if(!editTrapeze){
                //clearPoint();
                pointEdit = 0;
                editTrapeze=true;
            }
            else
                editTrapeze=false;
			break;
            
            case 'x':
            kinectWarp.saveFile("kinectMap.xml");
            break;
//        case 'y':
//            translateTrapeze(false, false);
//            break;
//        case 'h':
//            translateTrapeze(true, false);
//            break;
//        case 'g':
//            translateTrapeze(true, true);
//            break;
//        case 'j':
//            translateTrapeze(false, true);
//            break;
//        case 't':
//            moveSideTrapeze(false, true);
//            break;
//        case 'b':
//            moveSideTrapeze(false, false);
//            break;
//        case 'u':
//            moveSideTrapeze(true, true);
//            break;
//        case 'n':
//            moveSideTrapeze(true, false);
//            break;
        case 's':
            isSending = !isSending;
            break;

#ifndef MAC_OS_X_VERSION_10_7
        case 'p':
            vidGrabber.videoSettings();
            break;
#endif
            
        case' ':
        #ifdef _USE_LIVE_VIDEO
            useKinect = !useKinect;
        
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
    
    
    ptE = ofPoint(13,30);
    ptF = ofPoint(287,30);
    ptD = ofPoint(4,210);
    ptC = ofPoint(285, 210);
    ptTest = ofPoint(0, 160);
    pointEdit = 0;
    
    unzoomedCentre = ofPoint(globalWidth/2.0, globalHeight/2.0);
    unzoomedSize = 1.0f;
    
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
void testApp::moveSideTrapeze(bool onlyRight, bool onTheLeft){
    
    if( onlyRight){
        
        if(onTheLeft){
            
            ptF.x--;
            ptC.x--;
            
        }
        else{
            
            ptF.x++;
            ptC.x++;
            
        }
        
        
        
    }
    else{
        
        if(onTheLeft){
            
            ptE.x--;
            ptD.x--;
            
        }
        else{
            
            ptE.x++;
            ptD.x++;
            
        }
        
        
        
    }
    
    
    
}



//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    // Trapeze Editing
    if(!isFullScreen){
    if(editTrapeze)
    {
        
        // Warp into me
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
        
        
        // Warp into the entire stage
        if(ofGetMouseX() >400 && ofGetMouseX()<720 && ofGetMouseY() >20 && ofGetMouseY() < 260)
        {

                   unzoomedCentre  = ofPoint(ofGetMouseX()-400, ofGetMouseY()-20);
            
        }
        
        
        
    }
    }
    else{
        int dragWarpPttmp = kinectWarp.isOverCtl(ofPoint(ofGetMouseX(),ofGetMouseY()));
        //isDragging = dragWarpPt<0 && dragWarpPttmp>0;
        dragWarpPt=dragWarpPttmp;
    }
    
}

//-----------------

void testApp::mouseDragged( int x, int y,int button ){
    if(isFullScreen){   
if (dragWarpPt>=0){
    kinectWarp.dstPoint[dragWarpPt] = ofPoint(x,y);
    kinectWarp.updateMatrix();
}
    }
}

void testApp::mouseReleased( int x, int y,int button ){
    if(isFullScreen){
       // if(isDragging){
    dragWarpPt=-1;
            
    //}
     //   isDragging = false;
    }
}

void testApp::receiveMessage(ofxOscMessage &msg){
    
    

    
    
    // Set Blob min Size
    if(msg.getAddress()== "/BlobMin"){
        
        int size = msg.getArgAsInt32(0);
        
        if(size>0) blobMin = size;
        
    }
    else if(msg.getAddress()== "/cameraType"){
        
        useKinect= msg.getArgAsInt32(0)>0;
        
       
        
    }
    
    else if(msg.getAddress()== "/OSCSend"){
        
        isSending = msg.getArgAsInt32(0)==1;
        
    }
    
    // Send OSC sending
    else if(msg.getAddress()== "/BlobThresh"){
        if(msg.getArgAsInt32(0)>0) threshold = msg.getArgAsInt32(0);
               
    }
    
    
    
    
    
    
    
}


