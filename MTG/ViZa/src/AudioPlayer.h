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


#define POLYPHONY 10

class AudioPlayer{
public:
    
    static AudioPlayer * instance(){if(instance==NULL){inst = new AudioPlayer();}return inst;};
    
    
    
    
    
    AudioPlayer();
    
//    bool Play(Container & c);
//    void Stop(Container & c);
private:
    static AudioPlayer * inst;
    
    
    
};


#endif /* defined(__ViZa__AudioPlayer__) */
