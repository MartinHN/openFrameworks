#include "testApp.h"
//--------------------------------------------------------------
//                  VAISE - KINECT - ARDUINO - GUI
//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(28);
    
    globalWidth = 320;
    globalHeight = 240;
    
    angleB = -22;
    angleF = 21;
	
	// enable depth->video image calibration
	kinectF.setRegistration(true);
    
	kinectF.init();
	//globalinit(true); // shows infrared instead of RGB video image
	//globalinit(false, false); // disable video image (faster fps)
	
	kinectF.open();
    changeAngleKinect(true, angleF);

#ifdef USE_TWO_KINECT
    kinectB.setRegistration(true);
    kinectB.init(false, false);
    kinectB.open();
    changeAngleKinect(false, angleB);
    
#endif
        
    grayKinect.allocate(kinectF.width, kinectF.height);
    grayKinectWarped.allocate(globalWidth, globalHeight);
    grayThresImg.allocate(globalWidth, globalHeight);
    grayPrevious.allocate(globalWidth, globalHeight);
    colorImg.allocate(kinectF.width, kinectF.height);
    
    grayThreshNear.allocate(globalWidth, globalHeight);
	grayThreshFar.allocate(globalWidth, globalHeight);
    
    //ofParameters
    useTestVid.set("use Test Video",false);
    useTestVid.addListener(this,&testApp::loadVid);
    threshold.set("thresh near far", ofVec2f(255, 80),ofVec2f(0, 0),ofVec2f(255, 255));
    useMotionDetection.set("motion detection", false);
    useKinectFront.set("Use Kinect front", true);
    useVideo.set(" Use Video", false );
    erode.set("Erode", 0, 0, 10);
    blur.set("Blur", 0, 0, 10);
    angle.addListener(this, &testApp::changeAngle);
    save.addListener(this, &testApp::savePreset);
    load.addListener(this, &testApp::loadPreset);
    angle.set("Angle", 90, 4, 180);
    p1.set("p1", ofVec2f(0.0, 0), ofVec2f(0.0), ofVec2f(globalWidth, globalHeight));
    p2.set("p2", ofVec2f(globalWidth, 0), ofVec2f(0.0), ofVec2f(globalWidth, globalHeight));
    p3.set("p3", ofVec2f(globalWidth, globalHeight), ofVec2f(0.0), ofVec2f(globalWidth, globalHeight));
    p4.set("p4", ofVec2f(0.0, globalHeight), ofVec2f(0.0), ofVec2f(globalWidth, globalHeight));
    
    parameters.setName("settings");
    parameters.add(threshold);
    parameters.add(useMotionDetection);
    parameters.add(useKinectFront);
    parameters.add(useVideo);
    parameters.add(useTestVid);
    parameters.add(p1);
    parameters.add(p2);
    parameters.add(p3);
    parameters.add(p4);
    parameters.add(erode);
    parameters.add(blur);
    parameters.add(angle);
    
    gui.setup(); // most of the time you don't need a name
    
    gui.add(parameters);
    gui.add(labelPreset.setup("preset", ""));
    gui.add(preset.setup("preset", 0, 0, 15));
    gui.add(save.setup("Save"));
    gui.add(load.setup("Open"));
    
    // Syphon Server
    ofSetWindowTitle("kinect");
    syphonServerVid.setName("video");
    syphonServerBlob.setName("blob");
    
    //Serial
    int baud  = 9600;
//    serial.setup(0, baud); //open the first device
    
    //Arduino
    int a = 120;
    changeAngle(a);
    
    //OSC receiver
    oscReceiver.setup(12349);
    
    

    

    
    
}

//--------------------------------------------------------------
void testApp::update(){

    
    bool isNewFrame;
    ofxKinect* actualKinect;
#ifdef USE_TWO_KINECT
    if(useKinectFront) actualKinect = &kinectF;
    else actualKinect = &kinectB;
#else
    actualKinect = &kinectF;
#endif

    
    if(!useTestVid){
    if(useVideo){
        actualKinect->update();
        isNewFrame = actualKinect->isFrameNewVideo();
        
    }
    else{
        
        actualKinect->update();
        isNewFrame = actualKinect->isFrameNewDepth();
    }
    }
    else{
        player.update();
        isNewFrame = player.isFrameNew();
        
    }
    
    if(isNewFrame){
        if(!useTestVid){
      
        if(!useVideo){
            
            grayKinect.setFromPixels(actualKinect->getDepthPixels(), actualKinect->width, actualKinect->height);
            grayKinect.resize(globalWidth, globalHeight);

            //Trapeze correction
            grayKinectWarped = grayKinect;

            grayKinectWarped.warpPerspective(ofPoint(p1->x, p1->y),ofPoint(p2->x, p2->y),ofPoint(p3->x, p3->y),ofPoint(p4->x, p4->y));
            grayThresImg = grayKinectWarped;
            
            
            //Motion detection
            if(useMotionDetection)
            {
                grayThresImg.absDiff(grayPrevious);
                grayPrevious = grayKinectWarped;
                grayThreshNear = grayThresImg;
                grayThreshFar = grayThresImg;
                grayThreshNear.threshold(threshold->x, true);
                grayThreshFar.threshold(threshold->y);
            }
            else{
                
                grayThreshNear = grayThresImg;
                grayThreshFar = grayThresImg;
                int treshNear = threshold->x;
                int treshFar = threshold->y;
                
                grayThreshNear.threshold(treshNear, true);
                grayThreshFar.threshold(treshFar);
            }
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayThresImg.getCvImage(), NULL);

            
        }
        else{
            
            colorImg.setFromPixels(actualKinect->getPixels(),kinectF.width,kinectF.height);
            colorImg.resize(globalWidth, globalHeight);
            colorImg.warpPerspective(ofPoint(p1->x, p1->y),ofPoint(p2->x, p2->y),ofPoint(p3->x, p3->y),ofPoint(p4->x, p4->y));
            //colorImg.mirror(true, true);
            grayThresImg = colorImg;
            grayThresImg.threshold(threshold->y);
            
        }
        }
        else{
            colorImg.setFromPixels(player.getPixels(),player.width,player.height);
            grayThresImg = colorImg;
            grayThresImg.threshold(threshold->y,true);
        }
        
        float er = erode;
        float blu = int(blur / 2) * 2 +1;
        cvErode(grayThresImg.getCvImage(), grayThresImg.getCvImage(), NULL, er);
        cvDilate(grayThresImg.getCvImage(), grayThresImg.getCvImage(), NULL, er);
        
        if(blu>0)grayThresImg.blur(blu);
        
        
        //Syphon
        
        ofTexture tex = ofTexture(colorImg.getTextureReference());
        ofTexture tex2 = ofTexture(grayThresImg.getTextureReference());
        //syphonServerVid.publishTexture(&tex);
        syphonServerBlob.publishTexture(&tex2);
        
        
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw(){

    gui.draw();
    
    ofTranslate(240, 20);
    ofSetColor(255);
    
    
    if(!useVideo)
    {
        grayKinect.draw(0, 0, 320, 240 );
    }
    else
    {
        colorImg.draw(0, 0, 320, 240 );
    }
    
    ofFill();
    ofSetColor(255, 0, 0);
    ofCircle(p1->x, p1->y, 10);
    ofCircle(p2->x, p2->y, 10);
    ofCircle(p3->x, p3->y, 10);
    ofCircle(p4->x, p4->y, 10);

    ofTranslate(0, 260);
    ofSetColor(255);
    grayThresImg.draw(0, 0);
    
    
    
}
//-------------------------------------------------------------
void testApp::changeAngle(int  & i){
    
    if( i>4 && i< 180){
    
    unsigned char a = static_cast<char>(i);
    
    serial.writeByte(a);
        
    }
    
}

//--------------------------------------------------------------
void testApp::savePreset(){
    
    int numPreset = preset;
    string str = ofToString(numPreset);
    //str += ".xml";
    
    gui.saveToFile(str);
    
}

//--------------------------------------------------------------
void testApp::loadPreset(){
    
    int numPreset = preset;
    string str = ofToString(numPreset);
    //str += ".xml";
    
    gui.loadFromFile(str);
    
}
//-------------------------------------------------------------
void testApp::changeAngleKinect(bool useFront, int value){
    
    if(value>30) value = 30;
    
    
    if(useFront){
	kinectF.setCameraTiltAngle(value);
        
    }
    else{
        kinectB.setCameraTiltAngle(value);
    }
    
    
}
//-------------------------------------------------------------
void testApp::updateOsc(){
    
    ofxOscMessage m;
    
    if(oscReceiver.getNextMessage(&m))
    {
     
        if(m.getAddress()=="preset"){
            
            int pres = m.getArgAsFloat(0);
            preset = pres;
            
        }
        
    }
    
  

    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    
    
//    
//    switch (key) {
//        case OF_KEY_UP:
//            angleB++;
//            changeAngleKinect(false, angleB);
//            break;
//        case OF_KEY_DOWN:
//            angleB--;
//            changeAngleKinect(false, angleB);
//            break;
//            
//        default:
//            break;
//    }
    
    
}



void testApp::loadVid(bool & b){
    if(b){
        player.loadMovie("bien.mov");
        player.play();
    }
    else{
        player.close();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
