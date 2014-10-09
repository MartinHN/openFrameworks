//
//  AudioPlayer.h
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#ifndef __ViZa__AudioPlayer__
#define __ViZa__AudioPlayer__

#include <iostream>
#include "ofMain.h"
#include "Container.h"

#define POLYPHONY 10

class AudioPlayer{
public:
    
    static AudioPlayer * instance;
    
    std::map<ofSoundPlayer,int> players;
    
    
    
    AudioPlayer(){if(instance==NULL)instance = this;};
    
    void Play(Container & c);
    void Stop(Container & c);
    
    
    
    
};


#endif /* defined(__ViZa__AudioPlayer__) */
