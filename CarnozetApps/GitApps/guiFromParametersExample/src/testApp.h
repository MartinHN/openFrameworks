#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void exit();
    void deserialize(ofXml * x,ofParameterGroup * parent);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);		

	void circleResolutionChanged(int & circleResolution);
	void ringButtonPressed();

	bool bHide;
    ofParameterGroup allParams;
    list<ofAbstractParameter *> pL;

	ofxPanel gui;

	ofSoundPlayer ring;
};

