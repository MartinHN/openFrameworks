#pragma once



#define CAMCOLOR
//#define Debugcam
//#define localcamera
//#define testvid
//#define emulateshadow


//#define blobcomp

//#define syphonout

//#define isVisu




#define MOUSEATTR
#define blobosc

#include "ofMain.h"
#ifndef GUIMODE
#include "ofxOpenCv.h"
#endif
#include "ofxNetwork.h"
#include "ofxOsc.h"




#include "CamWiggler.h"

//#include "ofxXmlSettings.h"
//#include "ofx3DModelLoader.h"

//#include "ofxAssimpModelLoader.h"
//#include "ofxThreadedImageLoader.h"



//#include "VisuHandler.h"
#include "AttrCtl.h"



#include "Constants.h"
#ifdef GUIMODE
#include "Gui.h"
#include "ofxGui.h"
#endif
#include "visuWarper.h"





class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    
   
     void keyPressed  (int key);
#ifndef GUIMODE
    void exit();
    
    void windowResized(int w, int h);
    ofVec3f camToWorld(ofVec3f vecin);
    
#ifdef MOUSEATTR
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
#endif
    
    
    

    
    
    
    
#ifdef syphon  
    ofxSyphonClient blobClient;
#endif





 pingPongBuffer finalRender;

    
    

#if defined testvid || localcamera
    ofVideoGrabber vidGrab;
    ofVideoPlayer vidplay;
#endif
    
    
    
    
    
    

    
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
    string lastpath;
#endif
    

};
