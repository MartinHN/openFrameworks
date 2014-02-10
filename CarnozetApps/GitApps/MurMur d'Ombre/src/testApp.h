#pragma once



#define CAMCOLOR
//#define Debugcam
//#define localcamera
//#define testvid
//#define emulateshadow
//#define syphon

//#define blobcomp

//#define syphonout

//#define isVisu




#define MOUSEATTR
#define blobosc

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxNetwork.h"
#include "ofxOsc.h"
#include "ofxGui.h"




#if defined  syphon | defined syphonout
#include "ofxSyphon.h"
#endif





//#include "ofxXmlSettings.h"
//#include "ofx3DModelLoader.h"

//#include "ofxAssimpModelLoader.h"
//#include "ofxThreadedImageLoader.h"



//#include "VisuHandler.h"
#include "AttrCtl.h"
#include "CamWiggler.h"


#include "Constants.h"
#ifdef GUIMODE
#include "Gui.h"
#endif
#include "visuWarper.h"





class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    
   
    
#ifndef GUIMODE
    void exit();
     void keyPressed  (int key);
    void windowResized(int w, int h);
    ofVec3f camToWorld(ofVec3f vecin);
    
#ifdef MOUSEATTR
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
#endif
    
    
    

    
    
    
    
#ifdef syphon  
    ofxSyphonClient blobClient;
    ofxSyphonClient videoClient;
#endif
#ifdef syphonout  
    ofxSyphonServer syphonserver;
#endif





 pingPongBuffer finalRender;

    
    

#if defined testvid || localcamera
    ofVideoGrabber vidGrab;
    ofVideoPlayer vidplay;
#endif
    
    
    
    
    
    
    vector<ofImage> background;
    Boolean backTrack;
    ofPoint offsetbkg;
    float zoomback,curzoomback;
    float zoombackalpha;
#ifdef LIVEBLUR
//    vector<ofFbo*> foreground;
#else
    vector<ofImage> foreground;
#endif
    int backgroundtype,foregroundtype;
    
    ofCamera camera;
    
    int     width, height;
    int     imgWidth, imgHeight;
    
    
    
    //Colors
    
    int rback,gback,bback,alphablur,finalblur;
    int rblob,gblob,bblob,ablob;




    bool iscam;
    bool isFPS;
    



    //OSC
    ofxOscReceiver receiver;
    void oscUpdate();
    
    //Camera
    ofVec4f camrot;
    ofVec4f camdest;
    float alphacamrot;
    
    int camctl;


    
    CamWiggler camwiggle;
    
    //BLOB
#if  defined blobcomp || defined testvid
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
     Boolean bLearnBakground;
#endif
#ifdef blobosc
    
    
    bool drawBlob;
#endif
    pingPongBuffer polyBlob;
    int polyt,polylastt;
    
    
    bool drawcam;

    int threshold;
     ofVec2f centroid;

    
    
    //ATTRACTORS
    bool isAtt;
    
    
    

    int videow;
    int videoh;



    




    
    ofShader blurX;
    ofShader blurY;
    ofShader colorMod;

    ofPath polyline;

    

    bool isFullScreen;
    ofParameter<int> cropxl,cropxr,cropyh,cropyl;
 
#endif
    int zdepth;
    int inw;
    int inh;
    VisuHandler visuHandler;
    
    

    AttrCtl attrctl;
    

    float brightness,saturation,contrast;
    float brightness2,saturation2,contrast2;
    
    

#ifdef GUIMODE
    Gui gui;
#endif
    

};
