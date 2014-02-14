#pragma once



#define CAMCOLOR
//#define Debugcam
//#define localcamera
//#define testvid
//#define emulateshadow


//#define blobcomp

//#define syphonout

//#define isVisu





//#define blobosc

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
#include "CamHandler.h"
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
    void saveState(string & s);
    void loadState(string & s);
    
   
     void keyPressed  (int key);
#ifndef GUIMODE
    
    void exit();    
    void windowResized(int w, int h);
    

    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);


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
    
//    int rback,gback,bback,alphablur;
//    int rblob,gblob,bblob,ablob;




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
    ofShader bloom;
    ofShader gloom;

    ofPath polyline;

    

    bool isFullScreen;
    
 
#endif

    
        CamHandler camera2;
   
    ofParameterGroup globalParam;    
    ofParameterGroup settings;
    ofParameter<float> finalblur;
    ofParameter<float> brightness,saturation,contrast;
    ofParameter<int> rback,bback,gback;
    ofParameter<int> alphablur;
    ofParameter<float> brightness2,saturation2,contrast2;
    ofParameter<string> saveName;
    ofParameter<string> loadName;

    ofParameter<bool> isGloom;
    ofParameter<int> bloomsize;
    int zdepth;
    int inw;
    int inh;
    VisuHandler visuHandler;
    


    AttrCtl attrctl;
    
    ofxOscParameterSync paramSync;

    
    

#ifdef GUIMODE
    Gui gui;
    string lastpath;
#endif
    

};
