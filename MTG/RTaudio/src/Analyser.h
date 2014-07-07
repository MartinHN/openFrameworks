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
    
    virtual void analyze(vector<Slice> * v,Pooler * p){};
    virtual void registerParams(){
        MYPARAM(feature1,"envelope","","");
        MYPARAM(feature2,"nsdf","","");
        MYPARAM(feature3,"envelope","","");
        
    };
    
    ofParameterGroup settings;
    ofParameter<string> feature1,feature2,feature3;

    
    
};

class DirectAnalyzer : public Analyzer{
public:
    
//    DirectAnalyzer(Pooler * p):Analyzer(p){};
    void analyze(vector<Slice> * v,Pooler *p);
    void registerParams();
    ofParameter<int> x;
    ofParameter<int> y;
    ofParameter<int> z;
    
};




class AnalyzerH{
public:
    
    void setup(SlicerH * sH);
    bool analyzeIt();
    void registerParams();
    Analyzer * get(string n);
    SlicerH * sH;
    vector<string> getAnalyzerNames();
    
    vector<Analyzer*> analyzers;
    
    
    Analyzer * curAnalyzer;
    
    vector<Slice> * curSlice;
};





#endif /* defined(__RTaudio__Analyser__) */
