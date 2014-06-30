//
//  Viewer.h
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#ifndef __RTaudio__Viewer__
#define __RTaudio__Viewer__

#include <iostream>

#include "ofxUI.h"
#include "ofxGui.h"
#include "Analyser.h"
#include "Slicer.h"

class Viewer{
public:
    Viewer(){};
    void setup(AnalyzerH * a);
    
    void update();
    void updateAnalyzerGui();
    void updateSlicerGui();
    void updateView();
    void updateCache();
    void draw();

    void autoScale(bool & b);

    void resetView(bool & b);
    void setViewMouse(bool & b);
    
    void updateHoverSlice();
    
    void guiEvent(ofxUIEventArgs &e);
    void setupGui();
    
    void registerParams();
    
    ofEasyCam cam;
    
    ofxUIDropDownList *dl;
    ofxUISuperCanvas *guiconf;
    
    AnalyzerH * aH;
    
    bool isViewDirty,isCacheDirty,isGuiDirty;
    vector<ofVec3f> cache;
    
    ofParameter<bool> drawSlice,autoZoom,resetCam,viewMouse;
    ofParameter<ofVec3f> scale,center;
    ofParameterGroup settings;
    
    ofxPanel * guicam,* guia,*guis;
    

    bool displayFeatures;
    vector<ofColor> colors;
    int hoverIdx;
    
    vector<string> axesx,axesy,axesz;
    float viewR;
    
    
};

#endif /* defined(__RTaudio__Viewer__) */
