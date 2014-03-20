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
#include "Slicer.h"
#include "ofxUI.h"
#include "ofxGui.h"


class Viewer{
public:
    Viewer();
    
    void update();
    void updateGui();
    void updateView();
    void updateCache();
    void draw();
    void link2pool(Pooler * pin){p =pin;};
    void guiEvent(ofxUIEventArgs &e);
    void setupGui();
    
    void registerParams();
    
    ofEasyCam cam;
    
    ofxUIDropDownList *dl;
    ofxUISuperCanvas *gui;
    
    Slicer * slicer;
    
    bool isViewDirty,isCacheDirty,isGuiDirty;
    ofVbo vbo;
    
    ofParameter<bool> drawSlice;
    ofParameterGroup settings;
    
    ofxPanel * guip;
    
    vector<string> axesx,axesy,axesz;
    
    
};

#endif /* defined(__RTaudio__Viewer__) */
