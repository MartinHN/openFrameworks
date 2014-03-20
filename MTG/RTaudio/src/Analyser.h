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
#include "Pooler.h"
class Analyzer{
    
public:
//    Analyzer(Pooler * poolin):pool(poolin){};
    
    int nd;
    
    virtual void Analyze(vector<Slice> & v){};
    virtual void registerParams(){};
    
    ofParameterGroup settings;
    Pooler * pool;
    
    
};

class DirectAnalyzer : public Analyzer{
public:
    
//    DirectAnalyzer(Pooler * p):Analyzer(p){};
    void Analyze(vector<Slice> & v);
    void registerParams();
    ofParameter<int> x;
    ofParameter<int> y;
    ofParameter<int> z;
    
};





#endif /* defined(__RTaudio__Analyser__) */
