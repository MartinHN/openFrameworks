#pragma once

#include "ofMain.h"
#include "Physics.h"
#include "jsonLoader.h"
#include "GUI.h"
#include "Midi.h"

class ofApp : public ofBaseApp{

	public:
    
    static ofEasyCam cam;
    ofVec3f lastCamPos;
    bool isCamSteady;
    unsigned long long Casttime;
    
    float scrH ;
    float scrW;
    
    ofVec2f scrS;
    
    bool drawGUI;

    void drawMire();


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
        void isGUIing(bool & t);
    void drawCam();
    static void setcamOrtho(bool t);
    void loadFiles(string audiopath="",string segpath="");
    
};
