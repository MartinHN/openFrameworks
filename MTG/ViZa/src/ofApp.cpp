#include "ofApp.h"

ofEasyCam ofApp::cam;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
//    ofDisableSmoothing();
//    ofEnableAntiAliasing();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofDisableDepthTest();
    

    setcamOrtho(true);
   


    Casttime=ofGetElapsedTimeMillis();

    jsonLoader::instance()->loadSegments();
    
    ofBackground(0);
    

   
    Physics::updateVBO();
    
    
    GUI::instance()->setup();
    
    GUI::instance()->isModifiying.addListener(this, &ofApp::isGUIing);
    
    for(map<string,vector<Container* > >::iterator it = Container::songs.begin() ; it != Container::songs.end() ; ++it ){
        for(int i = 0 ; i <1 ; i++){
            AudioPlayer::Load(*it->second[i], true);
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
   
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    
    cam.begin();

    Physics::draw();

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
    
	ortho.makeOrthoMatrix(-1000,    1000   , -1000, 1000, .1, 2000);
    t.makeTranslationMatrix(0,0,-1000);
    
    ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadMatrix( ortho);
	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadMatrix( t);

    
    cam.getOrientationQuat().getRotate(angle, v);



    ofTranslate(910,-910,0);
    
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

        cam.setLensOffset(ofVec2f(-1,-1));
        cam.orbit(0,0, cam.getDistance());
        cam.setAutoDistance(false);
        cam.enableMouseInput();
        cam.enableMouseMiddleButton();
        Container::radius = 150;
        glPointSize(Container::radius);
        GLfloat attPoints[] = {0,Physics::distanceVanish(cam),0};//*,0};

        glPointParameterfv(	GL_POINT_DISTANCE_ATTENUATION,&attPoints[0]);
        
    }
    else{
        cam.disableOrtho();
        
        cam.setScale(1);
        cam.setNearClip(.001f);
        cam.setDistance(10);
        cam.setFarClip(cam.getDistance()*3);
        cam.setFov(2*ofRadToDeg(atan(.5/cam.getDistance())));
        cam.orbit(0,0, cam.getDistance());
        cam.setAutoDistance(false);
        cam.setLensOffset(ofVec2f(0,0));
        cam.enableMouseInput();
        cam.enableMouseMiddleButton();
        Container::radius = 150;
        glPointSize(Container::radius);
        GLfloat attPoints[] = {0,Physics::distanceVanish(cam),0};//*,0};
        
        glPointParameterfv(	GL_POINT_DISTANCE_ATTENUATION,&attPoints[0]);
        
        
    }
}

void ofApp::drawMire(){
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,0,0);
    ofNoFill();
    ofSetLineWidth(1);
    ofSetCircleResolution(60);
    ofCircle(ofVec3f(0),.5);
    ofRotateX(90);
    ofCircle(ofVec3f(0),.5);
    ofPopStyle();
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
        Container * cc = Physics::Cast(cam, ofVec2f(x,y),2.5);
        bool change = Container::hoverContainer(cc == NULL?-1:cc->index);
        Casttime = ofGetElapsedTimeMillis();
        if (change)GUI::LogIt(cc == NULL?"":cc->filename);
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(button==1){
     Physics::updateDrag(ofVec2f(x,y));
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(Container::hoverIdx==-1)return;
    Container * cc = &Container::containers[Container::hoverIdx];
    if (cc == NULL) return;
    if(button == 2)cc->state =cc->state==0?1:0;
    if(button == 1){
     Physics::dragged = cc;
    Physics::originDrag = cam.worldToCamera(Physics::vs[cc->index]).z;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    Physics::dragged = NULL;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
