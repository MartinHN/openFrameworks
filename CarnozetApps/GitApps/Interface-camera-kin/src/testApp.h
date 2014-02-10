#pragma once
#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxKinect.h"
#include "Warp.h"

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
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
        void receiveMessage(ofxOscMessage &m);
        int  recognizeGesture();
    void mouseDragged( int x, int y,int button );
    void mouseReleased( int x, int y,int button );
        ofPoint  unzoomedPolyline(ofPoint);
    
    
        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber            vidGrabber;
		#else
		  ofVideoPlayer             vidPlayer;
		#endif
        #ifdef MAC_OS_X_VERSION_10_7
        ofPtr<ofQTKitGrabber>       vidRecorder;

        #endif
    
    
        bool    isFullScreen;
        // Polyline
        int                         resample;
        vector<ofPoint>             extrem;
        ofxCvBlob                   bigBlob;
        ofPolyline                  blobLine;
        ofPolyline                  correctBlobLine;
        int                         blobMin;
    
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

		//Threshold
        int                         threshold;
        int                         diff_threshold;
    
        //Kinect
        bool                        useKinect;
        ofxKinect                   kinect;
        int                         nearThreshold;
        int                         farThreshold;
        ofxCvGrayscaleImage         grayThreshNear; // the near thresholded image
	    ofxCvGrayscaleImage         grayThreshFar; // the far thresholded image
        ofxCvGrayscaleImage         grayKinect;
    
    
        float                       thresDist;
        float                       simplify;
    
        //Recognize gesture
        float                       family_limit;
    
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
        void                        moveSideTrapeze(bool, bool);
        ofPoint                     unzoomedCentre;
        float                       unzoomedSize;
    
        
    

    
        // OSC send message : attractor + blob
        ofxOscSender            clientSender;
        string clientDestination; // IP address we're sending to
        int clientSendPort; // port we're sending to
        string clientTyping; // what we're going to send: some stuff you typed
        bool isSending;
    
        //OSC receive : change to ruban mode
        ofxOscReceiver          clientReceiver;
        
    
        Warp kinectWarp;
    int dragWarpPt;
    bool isDragging;

    
    

};

