//
//  Analyser.h
//  RTaudio
//
//  Created by martin hermant on 20/03/14.
//
//

#ifndef __RTaudio__Analyser__
#define __RTaudio__Analyser__

#include <iostream>
#include "ofMain.h"
#include "Misc.h"
#include "Slicer.h"

class Analyzer{
    
public:
//    Analyzer(Pooler * poolin):pool(poolin){};
    
    int nd;
    
    virtual void analyze(vector<Slice> & v){};
    virtual void registerParams(){};
    
    ofParameterGroup settings;
    Pooler * pool;
    
    
};

class DirectAnalyzer : public Analyzer{
public:
    
//    DirectAnalyzer(Pooler * p):Analyzer(p){};
    void analyze(vector<Slice> & v);
    void registerParams();
    ofParameter<int> x;
    ofParameter<int> y;
    ofParameter<int> z;
    
};




class AnalyzerH{
public:
    
    void setup(SlicerH * sH);
    void analyzeIt(string aname,string sname);
    void registerParams();
    Analyzer * get(string n);
    SlicerH * sH;
    vector<string> getAnalyzerNames();
    
    vector<Analyzer*> analyzers;
    
    vector<Slice> * curslice;
};





#endif /* defined(__RTaudio__Analyser__) */
