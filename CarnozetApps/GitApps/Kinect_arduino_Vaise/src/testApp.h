#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"

#define USE_TWO_KINECT

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
        void loadPreset();
        void savePreset();
    void loadVid(bool & b);
    void updateOsc();
    

    
    int globalWidth;
    int globalHeight;
    
    ofxKinect kinectF;      // kinect front
    ofxKinect kinectB;      // kinect back
    int angleF;
    void changeAngleKinect(bool useFront, int value);
    
    ofxPanel gui;
    //ofParameter<bool> vSync;
    ofParameterGroup parameters;
    ofXml settings;
    
    //ofParameters
    ofParameter<ofVec2f> p1;
    ofParameter<ofVec2f> p2;
    ofParameter<ofVec2f> p3;
    ofParameter<ofVec2f> p4;
    
    
    ofParameter<bool> useTestVid;
    ofParameter<bool> useKinectFront;
    ofParameter<bool> useVideo;
    ofParameter<bool> useMotionDetection;
    ofParameter<ofVec2f> threshold;
    ofParameter<int> erode;
    ofParameter<int> blur;
    ofParameter<int> angle;
    ofParameter<int> angleTilt;
    void changeAngle(int &i);
    void changeAngleTilt(int &i);
    ofxLabel labelPreset;
    ofxSlider<int> preset;
    ofxButton save;
    ofxButton load;
    
    
    //Serial
    ofSerial serial;
    
    
    //SYPHON
    ofxSyphonServer             syphonServerBlob;
    
    //Images
    ofxCvColorImage             colorImg;
    ofxCvGrayscaleImage         grayThresImg;
    ofxCvGrayscaleImage         grayPrevious;
    ofxCvGrayscaleImage         grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage         grayThreshFar; // the far thresholded image
    ofxCvGrayscaleImage         grayKinect;
    ofxCvGrayscaleImage         grayKinectWarped;
    
    //OSC
    ofxOscReceiver              oscReceiver;
    ofVideoPlayer player;

    
    
    
    
    
    
    
};
