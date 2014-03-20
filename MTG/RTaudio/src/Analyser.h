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

class Analyzer{
    
public:
    
    
    int dimension;
    
    virtual void Analyze(vector<slice> v){};
    
    
    
};





#endif /* defined(__RTaudio__Analyser__) */
