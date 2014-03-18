//
//  ofxEssentia.h
//  RTaudio
//
//  Created by martin hermant on 17/03/14.
//
//

#ifndef __RTaudio__ofxEssentia__
#define __RTaudio__ofxEssentia__

#include <iostream>
#include <fstream>
#include "ofMain.h"
#include "essentia/algorithmfactory.h"
#include "essentia/essentiamath.h"
#include "essentia/pool.h"

class ofxEssentia{
    public :
    ofxEssentia(){};
    
    void initalgo(string audioFilename ,string outputFilename);
    
    
    
};


#endif /* defined(__RTaudio__ofxEssentia__) */
