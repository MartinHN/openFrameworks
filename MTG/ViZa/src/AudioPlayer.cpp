//
//  AudioPlayer.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "AudioPlayer.h"
/*

bool AudioPlayer::Play(Container &c){
    if(players.size()<POLYPHONY){
        for(map<int,ofSoundPlayer>::iterator p= players.begin();p!=players.end();++p){
            if(p->first==c.index){
                p->second.stop();
                c.state=0;
                return false;
            }
            
        }
        
        players[c.index] = ofSoundPlayer();
        players[c.index].loadSound(c.path);
        players[c.index].setPositionMS(c.begin*1000.0);
        players[c.index].play();
    }
    
    return false;
}
*/