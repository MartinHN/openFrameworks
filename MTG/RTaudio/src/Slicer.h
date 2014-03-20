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




class Slicer{
    
public:
    
    virtual void SliceIt(vector<frame> &d){};
    virtual void registerParams(){};
    virtual map<string,int> outputs(){};
    
    
    vector<Slice> slices;
    
    ofParameterGroup settings;
    
};



class ThresholdSlicer : public Slicer{
public:


    void SliceIt(vector<frame> &d);
    void registerParams();
    map<string, int> outputs();
    
    ofParameter<float> threshold;
    
    
    
};
//class NoveltySlicer: public Slicer{
//    
//    
//    void SliceIt(vector<frame> d);
//    
//};

#endif /* defined(__RTaudio__Slicer__) */
