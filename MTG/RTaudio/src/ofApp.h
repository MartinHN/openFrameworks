#pragma once

#include "ofMain.h"
#include "Player.h"
#include "ofxJSONElement.h"
#include "Timed.h"
#include "Viewer.h"
#include "Pooler.h"
#include "Slicer.h"
#include "Analyser.h"



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
    void compute();

    
    void drawFromSlice();
    
    vector<Slice> slices;
    
    SlicerH sH;
    AnalyzerH aH;

    vector<Analyzer*> analyzers;
    
    int h;
    
    
    vector<Player *> player;
    int lastplayed;

    ofxJSONElement json;
    int classesN;
    float zoomfactor;
    
    Pooler pool;
    Viewer view;

		
};