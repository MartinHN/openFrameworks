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
    Viewer();
    
    void update();
    void updateGui();
    void updateView();
    void updateCache();
    void draw();
    void link2model(AnalyzerH * a){
        aH = a;
        
    };
    void setCurrentSlicer(int i);
    
    void guiEvent(ofxUIEventArgs &e);
    void setupGui();
    
    void registerParams();
    
    ofEasyCam cam;
    
    ofxUIDropDownList *dl;
    ofxUISuperCanvas *gui;
    
    AnalyzerH * aH;
    
    bool isViewDirty,isCacheDirty,isGuiDirty;
    vector<ofVec3f> cache;
    
    ofParameter<bool> drawSlice;
    ofParameterGroup settings;
    
    ofxPanel * guip;
    
    vector<ofColor> colors;
    
    vector<string> axesx,axesy,axesz;
    
    
};

#endif /* defined(__RTaudio__Viewer__) */
