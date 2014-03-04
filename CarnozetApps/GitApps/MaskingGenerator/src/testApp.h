#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Screen.h"
#include "ofxGui.h"

class testApp : public ofBaseApp{

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
    
        vector<Screen> listOfScreen;
        int activeScreen;
    
        void saveXml();
        void savePng();
    
        ofxXmlSettings xml;
    
        //GUI
        ofxPanel gui;
        ofxButton suppr;
        ofxButton add;
        ofxButton reset;
        ofxButton save;
    
        //Screen management
        void addScreen();
        void deleteScreen();
        void resetScreen();

    
		
};
