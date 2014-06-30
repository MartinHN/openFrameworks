//
//  Slicer.h
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//



#ifndef __RTaudio__Slicer__
#define __RTaudio__Slicer__

#include <iostream>
#include "ofMain.h"

#include "Misc.h"
#include "Pooler.h"




class Slicer{
    
public:
    
    virtual void SliceIt(vector<frame> *d){};
    virtual void registerParams(){};

    void clear();
    
    
    vector<Slice> slices;
    
    ofParameterGroup settings;
    
};



class ThresholdSlicer : public Slicer{
public:


    void SliceIt(vector<frame> * d);
    void registerParams();
    map<string, int> outputs();
    
    ofParameter<float> threshold;
    
    
    
};

class SlicerH{
public:
    void setup(Pooler * p);
    void registerParams();
    Slicer * get(string s);
    vector<string> getSlicerNames();
    void sliceIt();
    Pooler * pool;
    vector<Slicer * > slicers;
    
    Slicer * curSlicer;
    vector<frame> * curNovelty;
    
    
};
//class NoveltySlicer: public Slicer{
//    
//    
//    void SliceIt(vector<frame> d);
//    
//};

#endif /* defined(__RTaudio__Slicer__) */
