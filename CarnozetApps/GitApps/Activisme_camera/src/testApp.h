#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMacamPs3Eye.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"

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
    
        void updateContourFinder();
        void sendAttractors();
        void gainChanged(float& gain);
    
        // GUI
        ofxFloatSlider blobMin;
        ofxFloatSlider thres;
        ofxFloatSlider gain;
        ofxToggle isSending;
        ofxPanel gui;
    
        //PS3EYE
        ofxMacamPs3Eye ps3eye;
    
        //Image
        float globalW, globalH;
        float drawW, drawH;
        ofxCvColorImage  cam;
        ofxCvGrayscaleImage grey;
        ofxCvContourFinder          contourFinder;
    
        //OSC
        ofxOscSender sender;
    
        //ATTRACTORS
        ofPoint att1;
        bool is1;
        ofPoint att2;
        bool is2;
    
    
        
    

    
    
		
};
