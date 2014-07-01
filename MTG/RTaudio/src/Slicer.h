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
    
    virtual void SliceIt(LocalPool *d){};
    virtual void registerParams(){
        MYPARAM(novelty,"envelope","min","max");
    };

    void clear();
    
    
    vector<Slice> slices;
    
    ofParameterGroup settings;
    ofParameter<string> novelty;
    
    
};



class ThresholdSlicer : public Slicer{
public:


    void SliceIt(LocalPool * d);
    void registerParams();
    map<string, int> outputs();
    
    ofParameter<float> threshold;
    ofParameter<bool> invert;
    
    
    
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
