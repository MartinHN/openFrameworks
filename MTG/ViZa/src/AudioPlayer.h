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
class Container;


#define POLYPHONY 10



class AudioPlayer{
public:
    AudioPlayer();
    
    static AudioPlayer * instance(){
        if(inst==NULL){inst = new AudioPlayer();}return inst;
    };
    
    static std::map<int,ofSoundPlayer> players;
    static bool Play(Container & c,int s);
//    static bool Play(int uid,string path,float begin,float end ,ofParameter<float> & s);
    
    
    static AudioPlayer * inst;
    
    
    
};


#endif /* defined(__ViZa__AudioPlayer__) */
