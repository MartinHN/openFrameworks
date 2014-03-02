//
//  AutoTree.h
//  MursMurentDombres
//
//  Created by Aurélien Conil on 16/08/13.
//
//

#ifndef __MursMurentDombres__AutoTree__
#define __MursMurentDombres__AutoTree__
#include "VisuHandler.h"


#include <iostream>
#include "ofMain.h"
#include <vector.h>

class Branche{
    
    
public:
    
    Branche();
    Branche( ofPoint b, float a, int l, int total, bool isClock, ofImage* img);
    
    ofPoint begin;
    ofPoint end;
    float angle;
    float length;
    float finalLength;
    int totalLength;
    
    void draw();
    void update();
    
    
    bool isGrowing;
    bool isChildCreated;
    float lifeTime;
    
    ofPolyline shape;
    bool clockWise;
    
    ofImage* leaf;
    
    
};

//------------------------- Animation principale -----------------------


class AutoTree : public VisuClass{
public:
    AutoTree();
    AutoTree(VisuHandler * v);
    AutoTree(int w, int h, bool autostart, ofPath* pathin);
    
    
    void update(int w, int h);
    void draw(int w, int h);
    
    
    vector<Branche> list;
    
    ofFbo myfbo;
    
    void createBranche(ofPoint p, float a, int l,int tot,  bool isEnd);
    
    void reset( );
    
    
    void startStop(bool & start);
    void init();
    void clear();
    
    int width, height;
    
    int count;
    int introPhase;
   
    
    ofImage imagePart;
    
    ofPath* path;
    
     ofParameter<ofVec2f> pointToBegin;
    ofParameter<bool> initTrig;
    ofParameter<bool> addTrig;
    
};



#endif /* defined(__MursMurentDombres__AutoTree__) */
