//
//  VisuHandler.h
//  MurMur d'Ombre
//
//  Created by Tinmar on 14/08/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//
#pragma once
//#ifndef VISU_HANDLER
//#define VISU_HANDLER

#include "AttrCtl.h"
#include "Constants.h"
#include "ofxOscParameterSync.h"
#include "ofMain.h"
#include "Constants.h"
#include "VisuClass.h"
#include "Ecrans.h"
#include "ofXml.h"
#include "EcranHandler.h"

#include "listUtils.h"

//#define IS_VIDEO



class VisuHandler{
    
    
public:
    
    VisuHandler();
    VisuHandler(AttrCtl *attrctl,int inw,int inh,int zdepthin,int scrw,int scrh);
    ~VisuHandler(){FreeClear(visuList);};
    
    
    void addVisu(VisuClass * v);
    void setup(AttrCtl *attrctl,int inw,int inh,int zdepthin,int scrw,int scrh);
    void update();
    void updateHighFPS();
    const void draw();
    void registerParams();
    void saveState(string & s);
    void loadState(string & s);
    ofParameterGroup * getParamPtr();
    
    VisuClass * get(const string & name);



    


#ifdef HOMOGRAPHY
    ofFbo fbo;
#endif
    void loadScreensPos();
    const void printallp(ofParameterGroup p);
    int zdepth;
    int inw,inh,scrw,scrh;
    
    int beat;
    float audioenv;
    bool visufollowcam;
    
    AttrCtl* attr;
    ofTexture syphonTex;
    vector<ofPolyline> blobs;
    
    ScreenHandler sH;

    vector<VisuClass*> visuList;
//protected:
    
    ofParameterGroup allParams;
    ofxOscParameterSync paramSync;
    ofParameter<string> saveName;
    ofParameter<string> loadName;
  

  

    
};

