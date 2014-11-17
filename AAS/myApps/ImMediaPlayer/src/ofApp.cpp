#include "ofApp.h"

ofEvent<ofEventArgs> drawSyphonEvent;

//--------------------------------------------------------------
void ofApp::setup(){
    
#ifndef DEBUG
    //    ofSetDataPathRoot("../Resources/data/");
#endif
    

    // video
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofEnableAntiAliasing();

    ofSetFrameRate(60);
    
    outTexture.allocate(Screens::instance()->resolution.x, Screens::instance()->resolution.y);
    
    
    // keep it ordered
  
    //Screens
    Screens::instance();
 
    // GUI Init
    projects.init();
   
    // Syphon
    syphonOut.setName(APPNAME);
    ofSetBackgroundAuto(true);
    ofSetBackgroundColor(0);
#ifdef MOUSEDBG
    GloveOSC::gloves.push_back(new GloveInstance("mouse"));
    
#endif
    
//    ofSetWindowShape(Screens::instance()->resolution.x, Screens::instance()->resolution.y);
    windowResized(ofGetWindowWidth(), ofGetWindowHeight());
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
#ifdef SYPHON
    outTexture.begin();
#endif
    ofSetColor(0);
    ofVec2f res (Screens::instance()->resolution.x,Screens::instance()->resolution.y);
    ofRect(0,0,Screens::instance()->resolution.x,Screens::instance()->resolution.y);
    drawBackground(ofColor::gray, ofColor::black, OF_GRADIENT_BAR);
    ofSetColor(255);
    ofNotifyEvent(drawSyphonEvent,drawSyphon,this);
    
    ofDrawBitmapString(ofGetFrameRate(), 100,100);
    
#ifdef SYPHON
    outTexture.end();
    syphonOut.publishTexture(&outTexture.getTextureReference());
#endif
    
    ofSetColor(255,255,255,255);
    outTexture.draw(0,0,scrS.x,scrS.y);
}

#ifdef MOUSEDBG
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    touchEventArgs ev;
    // avoid keyrepeat
    if(!keys[key]){
   
        GloveInstance * curGlove = glove.getGlove("mouse");
    switch(key){
        case 'z':
            curGlove->setTouch(GLOVE_BUTTON_ZOOM, GLOVE_ACTION_DOWN);
            break;
            
        case 'd':
            curGlove->setTouch(GLOVE_BUTTON_DRAG, GLOVE_ACTION_DOWN);
            break;
            
        case 'l':
            projects.setCurrentDirectory(ofSystemLoadDialog("Load Project",true).filePath);
        default:
            break;
            
            
    }
        
        
    }
    relMouse = ofVec2f(mouseX,mouseY);
    keys[key] = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    touchEventArgs ev;
    
        GloveInstance * curGlove = glove.getGlove("mouse");
    switch(key){
        case 'z':
            curGlove->setTouch(GLOVE_BUTTON_ZOOM, GLOVE_ACTION_UP);
            break;
            
        case 'd':
            curGlove->setTouch(GLOVE_BUTTON_DRAG, GLOVE_ACTION_UP);
            break;
            
        case 'l':
            projects.setCurrentDirectory(ofSystemLoadDialog("Load Projects",true).filePath);
            break;
        default:
            break;
            
            
    }
    
    
    keys[key] = false;

}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y ){
    GloveInstance * curGlove = glove.getGlove("mouse");

    // avoid conflict when using mouse and glove at the same time
    if(!glove.isConnectedToServer){

        curGlove->setCursor2D(ofVec2f(x,y)*1.0/scrS);

        if(ofGetKeyPressed('z')){
            curGlove->setRelativeOrientation (ofVec3f(0,0,(ofGetMouseY()-relMouse.y)/4.0));
        }
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    GloveInstance * curGlove = glove.getGlove("mouse");
    curGlove->setCursor2D(ofVec2f(x,y)*1.0/scrS);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    GloveInstance * curGlove = glove.getGlove("mouse");
    curGlove->setCursor2D(ofVec2f(x,y)*1.0/scrS);
    curGlove->setTouch(GLOVE_BUTTON_CLICK, GLOVE_ACTION_DOWN);
    lastMousePress = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    GloveInstance * curGlove = glove.getGlove("mouse");
    curGlove->setCursor2D(ofVec2f(x,y)*1.0/scrS);
    curGlove->setTouch(GLOVE_BUTTON_CLICK, GLOVE_ACTION_UP);
    if(ofGetElapsedTimef()-lastMousePress>.15){
        curGlove->setTouch(GLOVE_BUTTON_CLICK,GLOVE_ACTION_LONGPRESS);
    }
    else{
        curGlove->setTouch(GLOVE_BUTTON_CLICK,GLOVE_ACTION_SHORTPRESS);
    }
    
    
    
}
#endif
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    scrS.set(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

void ofApp::exit(){
    
    glove.unregisterOSC();
}


void ofApp::drawBackground(ofColor start, ofColor end, ofGradientMode mode){
    
    
    float w = Screens::instance()->resolution.x, h = Screens::instance()->resolution.y;
    
    
    
    gradientMesh.clear();
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    gradientMesh.setUsage(GL_STREAM_DRAW);
    if(mode == OF_GRADIENT_CIRCULAR) {
        // this could be optimized by building a single mesh once, then copying
        // it and just adding the colors whenever the function is called.
        ofVec2f center(w / 2, h / 2);
        gradientMesh.addVertex(center);
        gradientMesh.addColor(start);
        int n = 32; // circular gradient resolution
        float angleBisector = TWO_PI / (n * 2);
        float smallRadius = ofDist(0, 0, w / 2, h / 2);
        float bigRadius = smallRadius / cos(angleBisector);
        for(int i = 0; i <= n; i++) {
            float theta = i * TWO_PI / n;
            gradientMesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
            gradientMesh.addColor(end);
        }
    } else if(mode == OF_GRADIENT_LINEAR) {
        gradientMesh.addVertex(ofVec2f(0, 0));
        gradientMesh.addVertex(ofVec2f(w, 0));
        gradientMesh.addVertex(ofVec2f(w, h));
        gradientMesh.addVertex(ofVec2f(0, h));
        gradientMesh.addColor(start);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
    } else if(mode == OF_GRADIENT_BAR) {
        gradientMesh.addVertex(ofVec2f(w / 2, h / 2));
        gradientMesh.addVertex(ofVec2f(0, h / 2));
        gradientMesh.addVertex(ofVec2f(0, 0));
        gradientMesh.addVertex(ofVec2f(w, 0));
        gradientMesh.addVertex(ofVec2f(w, h / 2));
        gradientMesh.addVertex(ofVec2f(w, h));
        gradientMesh.addVertex(ofVec2f(0, h));
        gradientMesh.addVertex(ofVec2f(0, h / 2));
        gradientMesh.addColor(start);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
        gradientMesh.addColor(start);
    }
    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
    glDepthMask(GL_FALSE);
    //    glBegin(OF_PRIMITIVE_TRIANGLE_FAN);
    gradientMesh.draw();
    if(depthMaskEnabled){
        glDepthMask(GL_TRUE);
    }
}
