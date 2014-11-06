#include "ofApp.h"

ofEvent<ofEventArgs> drawSyphonEvent;

//--------------------------------------------------------------
void ofApp::setup(){
    
#ifndef DEBUG
    //    ofSetDataPathRoot("../Resources/data/");
#endif
    
    
    ofEnableAlphaBlending();
    cout << PROJECTPATH << endl;
    cout << ofToDataPath("/lala") << endl;
    ofSetFrameRate(30);
    
    outTexture.allocate(Screens::instance()->resolution.x, Screens::instance()->resolution.y);
    
    
    // GUI Init
    projects.init();
    projects.startWatch();
    
    
    
    
    // Syphon
    syphonOut.setName(APPNAME);
    
    
    //Screens
    Screens::instance();
    
    
    
    // Background
    ofSetBackgroundAuto(true);
    
    back.loadImage("mire.png");
    
    GloveOSC::gloves.push_back(new GloveInstance("mouse"));
    
    
    
    
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
    //    back.width = res.x;
    //    back.height = res.y;
    //    back.draw(0,0);
    drawBackground(ofColor::gray, ofColor::black, OF_GRADIENT_BAR);
    ofSetColor(255);
    ofNotifyEvent(drawSyphonEvent,drawSyphon,this);
    
    
#ifdef SYPHON
    outTexture.end();
    syphonOut.publishTexture(&outTexture.getTextureReference());
#endif
    
    ofSetColor(255);
    outTexture.draw(0,0,ofGetScreenWidth(),ofGetScreenHeight());
}

#ifdef MOUSEDBG
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    touchEventArgs ev;
    // avoid feedback loop when emulating glove events with mouse as glove events are already wrapped to mouse events ->GUIgloveWrapper
    if(!isEventing){
        isEventing=true;
        GloveInstance * curGlove = glove.getGlove("mouse");
    switch(key){
        case 'z':
            ev.gloveId = curGlove;
            ev.touchId = GLOVE_ZOOM;
            ev.state = GLOVE_DOWN;
            ofNotifyEvent(curGlove->touchEvent,ev, this);
            break;
            
        case 'd':
            ev.gloveId =curGlove;
            ev.touchId = GLOVE_DRAG;
            ev.state = GLOVE_DOWN;
            ofNotifyEvent(curGlove->touchEvent,ev, this);
            break;
            
            
        default:
            break;
            
            
    }
        isEventing=false;
    }
    relMouse = ofVec2f(ofGetMouseX(),ofGetMouseY());
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    touchEventArgs ev;
    if(!isEventing){
        isEventing=true;
        GloveInstance * curGlove = glove.getGlove("mouse");
    switch(key){
        case 'z':
            ev.gloveId =curGlove;
            ev.touchId = GLOVE_ZOOM;
            ev.state = GLOVE_UP;
            ofNotifyEvent(curGlove->touchEvent,ev, this);
            break;
            
        case 'd':
            ev.gloveId =curGlove;
            ev.touchId = GLOVE_DRAG;
            ev.state = GLOVE_UP;
            ofNotifyEvent(curGlove->touchEvent,ev, this);
            break;
            
            
        default:
            break;
            
            
    }
        isEventing=false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y ){
    GloveInstance * curGlove = glove.getGlove("mouse");
    // avoid conflict when using mouse and glove at the same time
    if(!glove.isConnectedToServer){
        
        
        
        if(!isEventing){
            isEventing=true;
        
        
        pair<GloveInstance*,ofVec2f> ev(curGlove,ofVec2f(mouseX,mouseY));
        ofNotifyEvent(curGlove->cursor2DEvent,ev,this);
        
            isEventing=false;
        }
        
        
        // any keypress emulates relative orientation events almost as in real world...
        if(ofGetKeyPressed()){
            std::pair<GloveInstance*,ofVec3f> ev2(curGlove,ofVec3f(0,0,(ofGetMouseY()-relMouse.y)/400.0));
            ofNotifyEvent(curGlove->relativeOrientationEvent, ev2, this);
        }
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    GloveInstance * curGlove = glove.getGlove("mouse");
    if(!isEventing){
        isEventing=true;
    // still sends events when dragging
    std::pair<GloveInstance*,ofVec2f> ev(curGlove,ofVec2f(x,y));
    ofNotifyEvent(curGlove->cursor2DEvent,ev, this);
        isEventing=false;
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    GloveInstance * curGlove = glove.getGlove("mouse");
    if(!isEventing){
    isEventing=true;
    touchEventArgs ev;
    ev.gloveId =curGlove;
    ev.state = GLOVE_DOWN;
    ev.touchId = GLOVE_CLICK;
    ofNotifyEvent(curGlove->touchEvent,ev, this);
 
        isEventing=false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    GloveInstance * curGlove = glove.getGlove("mouse");
    if(!isEventing){
        isEventing=true;
    
    touchEventArgs ev;
    ev.gloveId =curGlove;
    ev.state = GLOVE_UP;
    ev.touchId = GLOVE_CLICK;
    ofNotifyEvent(curGlove->touchEvent,ev, this);
    
        isEventing=false;
    }
    
    
}
#endif
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
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
