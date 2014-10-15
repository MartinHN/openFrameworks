#include "ofApp.h"

ofEasyCam ofApp::cam;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
//    cam.orbit(0, 0,3);
    cam.setNearClip(.01f);
    cam.setDistance(100);
    cam.setFov(ofRadToDeg(2*atan(.5/cam.getDistance())));
    cam.orbit(0,0, cam.getDistance());
    cam.setAutoDistance(false);
    cam.enableMouseInput();
    cam.disableMouseMiddleButton();
//
    Casttime=ofGetElapsedTimeMillis();

    jsonLoader::instance()->loadSegments();
    
    ofBackground(255);
    
    Container::radius = cam.getDistance()*15;
    glPointSize(Container::radius);
    GLfloat attPoints[] = {0,Physics::distanceVanish(cam),0};//*,0};

    glPointParameterfv(	GL_POINT_DISTANCE_ATTENUATION,&attPoints[0]);


   
    Physics::updateVBO();
    
    
    GUI::instance()->setup();
    
    GUI::instance()->isModifiying.addListener(this, &ofApp::isGUIing);
}

//--------------------------------------------------------------
void ofApp::update(){
    
   
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    
    cam.begin();
//    glBegin(GL_POINT);
    ofSetColor(ofColor(255,0,0));
    Physics::vbo.drawElements(GL_POINTS,Physics::vbo.getNumVertices());
    drawMire();
    cam.end();
    
     //if(drawGUI)GUI::instance()->draw();
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
        cam.disableMouseMiddleButton();
    }
    else{
        cam.enableMouseInput();
        cam.enableMouseMiddleButton();
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'g':
            GUI::instance()->guiconf->toggleMinified();
            break;
        case 'x':
            cam.orbit(90,0,cam.getDistance());
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
        Container::hoverContainer(cc == NULL?-1:cc->index);
        Casttime = ofGetElapsedTimeMillis();
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
    Container * cc = Physics::Cast(cam, ofVec2f(x,y));
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
