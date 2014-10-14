#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
//    cam.orbit(0, 0,3);
    cam.setNearClip(.01f);
    cam.setFov(ofRadToDeg(2*atan(.5)));
    cam.orbit(0,0, 1);
    cam.setAutoDistance(false);
    cam.enableMouseInput();
    cam.enableMouseMiddleButton();
//
    

    jsonLoader::instance()->loadSegments();
    
    
    
    Container::radius = 15;
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
    ofSetColor(255,255,0);
    ofNoFill();
    ofSetLineWidth(1);
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
            
        default:
            break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    Container * cc = Physics::Cast(cam, ofVec2f(x,y));
    if (cc == NULL) return;
    if(button == 0)cc->state =cc->state==0?1:0;;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
