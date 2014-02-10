#pragma once
#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxOsc.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
#define LOCALOSC
//#define DEBUG_EXTREM

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
        void findExtrem(ofxCvBlob shape, float simpl);
        float calcAngle(ofVec2f, ofVec2f );
        int  sendPolylineMessage(bool isBlob);
        int  sendAttractorMessage();
        void receiveMessage();
        int  recognizeGesture();
        ofPoint correctTrapeze(ofPoint);
    
    
        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber            vidGrabber;
		#else
		  ofVideoPlayer             vidPlayer;
		#endif
        #ifdef MAC_OS_X_VERSION_10_7
        ofPtr<ofQTKitGrabber>       vidRecorder;

#endif
        // Polyline
        int                         resample;
        vector<ofPoint>             extrem;
        ofxCvBlob                   bigBlob;
        ofPolyline                  blobLine;
        ofPolyline                  correctBlobLine;
        int                         blobMin;
        bool                        rubanMode;
    
        int    lastPolyt,polytime;
    
        // Debug addon
        #ifdef DEBUG_EXTREM
        int                         currPoint;
        ofPoint                     currPointToDraw;
        float                       currAngle;
        float                       currNextLength;
        #endif

        // Images
        int                         globalWidth;
        int                         globalHeight;
        ofxCvColorImage             colorImg;
        ofxCvGrayscaleImage         grayThresImg;

        ofxCvContourFinder          contourFinder;

		int                         threshold;
        int                         diff_threshold;
        int                         diff_mhi;
    
        float                       thresDist;
        float                       simplify;
    
        int                         forceCount;
        ofPoint                     family1;
        ofPoint                     family2;
        ofPoint                     family2_2;
        ofPoint                     family3;
    
        // Trapeze Correction
        ofPoint                     ptE;
        ofPoint                     ptF;
        ofPoint                     ptC;
        ofPoint                     ptD;
        ofPoint                     ptTest;
        bool                        editTrapeze;
        int                         pointEdit;
        void                        clearPoint();
        void                        translateTrapeze(bool, bool);
    
        // Find extrem
        float sureLimit;
        float doubtLimit;
    
        // OSC send message : attractor + blob
        ofxOscSender            clientSender;
        string clientDestination; // IP address we're sending to
        int clientSendPort; // port we're sending to
        string clientTyping; // what we're going to send: some stuff you typed
        bool isSending;
    
        //OSC receive : change to ruban mode
        ofxOscReceiver          clientReceiver;


    
    

};

