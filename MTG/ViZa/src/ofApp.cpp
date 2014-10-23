#include "ofApp.h"
//#include <omp.h>
#include <stdio.h>
ofEasyCam ofApp::cam;

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    
//
//#pragma omp parallel
//        printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
    
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
//    ofDisableSmoothing();
//    ofEnableAntiAliasing();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofDisableDepthTest();
    

    setcamOrtho(true);
   


    Casttime=ofGetElapsedTimeMillis();

    
    loadFiles();

    
    ofBackground(0);
    
    ofShowCursor();
   

    
    
    
    
    GUI::instance()->isModifiying.addListener(this, &ofApp::isGUIing);
    
    scrH =ofGetScreenHeight();
    scrW =ofGetScreenWidth();

}

//--------------------------------------------------------------
void ofApp::update(){
    Midi::update();
   
}

void ofApp::loadFiles(string audiopath,string segpath){
    AudioPlayer::UnloadAll();
    jsonLoader::instance()->loadSegments(audiopath,segpath);
    Physics::resizeVBO();
    GUI::instance()->registerListener();
    GUI::instance()->setup();
    
    for(map<string,vector<Container* > >::iterator it = Container::songs.begin() ; it != Container::songs.end() ; ++it ){
        for(int i = 0 ; i <POLYPHONY ; i++){
            AudioPlayer::Load(*it->second[i], true);
        }
    }
   
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    if(cam.getOrtho()){

        ofRectangle VP(0-1.0/(2.0*cam.getDistance()-1) ,-1.0/(2.0*cam.getDistance())-1, ofGetViewportWidth()+1.0/cam.getDistance()-1,ofGetViewportHeight()+1.0/cam.getDistance()-1 );
        cam.begin();
    }
    else{
    cam.begin();
    }
    Physics::draw();
    Midi::draw();
    drawMire();

    cam.end();
    
   drawCam();
    
     //if(drawGUI)GUI::instance()->draw();
}

void ofApp::drawCam(){

    ofPushMatrix();
    ofPushView();
    ofEnableDepthTest();
    ofMatrix4x4 t ;
    ofMatrix4x4 ortho ;
    float angle;
    ofVec3f v;

	ortho.makeOrthoMatrix(-scrW/2,    scrW/2   , -scrH/2, scrH/2, .1, 2000);
    t.makeTranslationMatrix(0,0,-1000);
    
    ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadMatrix( ortho);
	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadMatrix( t);

    
    cam.getOrientationQuat().getRotate(angle, v);



    ofTranslate(scrW/2 - 90,-scrH/2 + 90,0);
    
    ofRotate(angle,v.x,-v.y,v.z);
//    ofRotateX(180);
    
    ofDrawAxis(50);
    

    
    ofPopMatrix();
    ofPopView();
}


void ofApp::setcamOrtho(bool t){
    


    
  

    if(t){
        cam.enableOrtho();
        cam.setNearClip(.000001f);
    
        
        cam.setScale(1.0/ofGetViewportHeight());
        cam.setDistance(1);//ofGetScreenHeight());
        cam.setFarClip(cam.getDistance()*2*ofGetScreenHeight());
        cam.setNearClip(.000001f);
        cam.setLensOffset(ofVec2f(-1,-1));
        cam.orbit(0,0, cam.getDistance());
        cam.setAutoDistance(false);
        cam.enableMouseInput();
        cam.enableMouseMiddleButton();
        Container::radius = 300;
        glPointSize(Container::radius);
        GLfloat attPoints[] = {0,Physics::distanceVanish(cam),0};//*,0};

        glPointParameterfv(	GL_POINT_DISTANCE_ATTENUATION,&attPoints[0]);
        
    }
    else{
        cam.disableOrtho();
        
        cam.setScale(1);
        cam.setNearClip(.000001f);
        cam.setDistance(1);
        cam.setFarClip(cam.getDistance()*3);
        cam.setFov(2*ofRadToDeg(atan(.5/cam.getDistance())));
        cam.orbit(0,0, cam.getDistance());
        cam.setAutoDistance(false);
        cam.setLensOffset(ofVec2f(0,0));
        cam.enableMouseInput();
        cam.enableMouseMiddleButton();
        Container::radius = 50;
        glPointSize(Container::radius);
        GLfloat attPoints[] = {0,Physics::distanceVanish(cam),0};//*,0};
        
        glPointParameterfv(	GL_POINT_DISTANCE_ATTENUATION,&attPoints[0]);
        
        
    }
}

void ofApp::drawMire(){
    ofPushMatrix();
    ofPushStyle();
    
//    if(ofApp::cam.getOrtho())ofScale(1.0/ofApp::cam.getDistance(),1.0/ofApp::cam.getDistance(),1.0/ofApp::cam.getDistance());
    
    ofNoFill();
    ofSetLineWidth(1);
    ofSetCircleResolution(60);
    

    
    
    ofSetColor(0,0,255);
    ofCircle(ofVec3f(0),.5);

    
    ofRotateX(90);
    ofSetColor(0,255,0);
    ofCircle(ofVec3f(0),.5);

    
    ofRotateY(90);
    ofSetColor(255,0,0);
    ofCircle(ofVec3f(0),.5);


    
    ofPopStyle();
    ofPopMatrix();
    
    ofPushMatrix();
    for (int i = 0 ; i < 3 ; i++){
        ofVec3f mask(i==0?255:0,i==1?255:0,i==2?255:0);
        ofSetColor (mask.x,mask.y,mask.z);
        ofDrawBitmapString(GUI::instance()->attr[i]->getSelected()[0]->getName(), .45/255.*mask);
    }
    ofPopMatrix();
}



void ofApp::isGUIing(bool & t){
    if(t){
        cam.disableMouseInput();

    }
    else{
        cam.enableMouseInput();
        
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'x':
            cam.orbit(-90,0,cam.getDistance());
            break;
        case 'y':
            cam.orbit(0,90,cam.getDistance());
            break;
        case 'z':
            cam.orbit(0,0,cam.getDistance());
            break;
            
        case 'l':{
            ofFileDialogResult f = ofSystemLoadDialog("analysisFiles",true);
            loadFiles("",f.filePath);
            break;
    }
        default:
            break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if(ofGetElapsedTimeMillis()-Casttime>70){
        Container * cc = Physics::Cast(cam, ofVec2f(x,y),1.,GUI::instance()->selBrightest->getValue());
        bool change = Container::hoverContainer(cc == NULL?-1:cc->index);
        Casttime = ofGetElapsedTimeMillis();
        if (change)GUI::LogIt(cc == NULL?"":cc->filename +" : "+ ofToString((cc->getPos()*(Physics::maxs.get()-Physics::mins)+Physics::mins)));
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(button==1){
        if(Physics::updateDrag(ofVec2f(x,y))){
            
        }

    }

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(Container::hoverIdx==-1)return;
    Container * cc = &Container::containers[Container::hoverIdx];
    if (cc == NULL) {

        return;}
    if(button == 2)cc->state =cc->state==0?1:0;
    if(button == 1){
    cam.disableMouseMiddleButton();
    Physics::dragged = cc;
    Physics::originDrag = cam.worldToCamera(Physics::vs[cc->index]).z;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    Physics::dragged = NULL;
    cam.enableMouseMiddleButton();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
   scrH =ofGetScreenHeight();
    scrW =ofGetScreenWidth();
    setcamOrtho(cam.getOrtho());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


