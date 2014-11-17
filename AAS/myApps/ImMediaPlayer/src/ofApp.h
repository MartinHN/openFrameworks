#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "Screens.h"
#include "GUIProjects.h"
#include "GUIConfig.h"

#include "GUIGloveWrapper.h"

#include "ofxTweener.h"




class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
        void mouseMoved( int x, int y  );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();

    

    ofFbo outTexture;
    ofxSyphonServer syphonOut;

    
    

    
    // keep it ordered !!!
    ofEventArgs drawSyphon;
    GloveOSC glove;
    GUIProjects guiProjects;
    GUIConfig guiConfig;
    
    GUIGloveWrapper gloveWrapper;

    
    
#ifdef MOUSEDBG
    ofVec2f relMouse;
    bool isEventing=false;
    float lastMousePress=0;
        map<char,bool> keys;
#endif
    ofVec2f scrS;
    ofVboMesh gradientMesh;
    void drawBackground(ofColor start, ofColor end, ofGradientMode mode);
    
    ofImage back;
   

};
