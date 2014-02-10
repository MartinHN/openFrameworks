//
//  AutoTree.h
//  MursMurentDombres
//
//  Created by Aurélien Conil on 16/08/13.
//
//

#ifndef __MursMurentDombres__AutoTree__
#define __MursMurentDombres__AutoTree__

#include <iostream>
#include "ofMain.h"
#include <vector.h>

class Branche{
    
    
public:
    
    Branche();
    Branche( ofPoint b, float a, int l, bool isClock, ofImage* img);
    
    ofPoint begin;
    ofPoint end;
    float angle;
    float length;
    float finalLength;
    
    void draw();
    void update();
    
    
    bool isGrowing;
    bool isChildCreated;
    int lifeTime;
    
    ofPolyline shape;
    bool clockWise;
    
    ofImage* leaf;
    
    
};

//------------------------- Animation principale -----------------------


class AutoTree{
public:
    AutoTree();
    AutoTree(int w, int h, bool autostart);
    void draw();
    
    
    vector<Branche> list;
    
    ofFbo myfbo;
    
    void createBranche(ofPoint p, float a, int l, bool isEnd);
    
    void reset( ofPoint);
    
    void startStop( bool start);
    void init();
    void clear();
    
    int width, height;
    
    int count;
    
    ofImage imageLeaf;
    
};



#endif /* defined(__MursMurentDombres__AutoTree__) */
