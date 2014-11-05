#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"
#include "Screens.h"
#include "GUIProjects.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
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
    Cursor glove;
    Screens screens;
    GUIProjects projects;
    
    

    
    
#ifdef MOUSEDBG
    ofVec2f relMouse;
#endif
    
    static ofVboMesh gradientMesh;
    void drawBackground(ofColor start, ofColor end, ofGradientMode mode);
    
    ofImage back;
   
};
