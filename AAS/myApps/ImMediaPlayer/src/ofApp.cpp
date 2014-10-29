#include "ofApp.h"

ofEvent<ofEventArgs> drawSyphonEvent;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);

    outTexture.allocate(Screens::instance()->resolution.x, Screens::instance()->resolution.y);
    
    projects.startWatch();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

#ifdef SYPHON
    outTexture.begin();
#endif
   
    
    ofNotifyEvent(drawSyphonEvent,drawSyphon,this);
    
    
    ofSetColor(0);
    ofDrawBitmapString("lala", 50, 50);
    
    
    
#ifdef SYPHON
    outTexture.end();
    syphonOut.publishTexture(&outTexture.getTextureReference());
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
#ifdef MOUSEDBG
    
    std::pair<Cursor*,ofVec2f> ev(NULL,ofVec2f(x,y));
    ofNotifyEvent(glove.cursor2DEvent,ev, this);
    
#endif
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    #ifdef MOUSEDBG
    std::pair<Cursor*,ofVec2f> ev(NULL,ofVec2f(x,y));
    ofNotifyEvent(glove.cursor2DEvent,ev, this);
    #endif
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
#ifdef MOUSEDBG
    touchEventArgs ev;
    ev.gloveId = NULL;
    ev.touchId = A_DRAG;
    ev.type = T_DOWN;
    ofNotifyEvent(glove.touchEvent,ev, this);
    ev.touchId = A_CLICK;
    ofNotifyEvent(glove.touchEvent,ev, this);
#endif
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

#ifdef MOUSEDBG
    touchEventArgs ev;
    ev.gloveId = NULL;
    ev.touchId = A_DRAG;
    ev.type = T_UP;
    ofNotifyEvent(glove.touchEvent,ev, this);
    ev.touchId = A_CLICK;
    ofNotifyEvent(glove.touchEvent,ev, this);
    
#endif
    
    
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
