#pragma once

#include "ofMain.h"
#include "Player.h"
#include "ofxJSONElement.h"
#include "Timed.h"
#include "ofxEssentia.h"



class Slice{
public:
    Slice(float a, float b, int t):start(a),length(b),type(t){};
    float start;
    float length;
    int type;
};

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

    
    
    void loadSlices(string filename);
    
    void drawFromSlice();
    
    vector<Slice> slices;
    int h;
    
    
    Player player;
    int lastplayed;

    ofxJSONElement json;
    int classesN;
    float zoomfactor;
    
    ofxEssentia ess;
		
};
