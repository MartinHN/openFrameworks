#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
//    cam.orbit(0, 0,3);
    cam.setNearClip(.01f);
    cam.setFov(ofRadToDeg(2*atan(.5)));
    cam.orbit(0,0, 1);
    
    
    

    jsonLoader::instance()->loadSegments();
    
    Container::radius = 30;
    glPointSize(Container::radius);
    GLfloat attPoints[] = {0,Container::distanceVanish(cam),0};//*,0};

    glPointParameterfv(	GL_POINT_DISTANCE_ATTENUATION,&attPoints[0]);


   
    Container::updateVBO();
}

//--------------------------------------------------------------
void ofApp::update(){
    
   
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    
    cam.begin();
//    glBegin(GL_POINT);
    ofSetColor(ofColor(255,0,0));
    Container::vbo.drawElements(GL_POINTS,Container::vbo.getNumVertices());
    drawMire();
    cam.end();
    
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




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
    if(button == 0)Container::Cast(cam, ofVec2f(x,y));
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
