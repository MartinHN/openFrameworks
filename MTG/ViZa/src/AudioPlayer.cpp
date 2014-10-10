//
//  AudioPlayer.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "AudioPlayer.h"


std::map<int,ofSoundPlayer>  AudioPlayer::players;
AudioPlayer * AudioPlayer::inst;

AudioPlayer::AudioPlayer(){
    
}


bool AudioPlayer::Play(Container & c, int s){
    
    
    if(players.size()>POLYPHONY && s>0)return false;
    
    
    for(map<int,ofSoundPlayer>::iterator p= players.begin();p!=players.end();++p){
        if(p->first==c.index){
            if(s ==1){
                p->second.play();
                s=0;
                players.erase(p++);
                
                return false;
            }
            else if( s ==0){
                p->second.stop();
                players.erase(p++);
                return false;
                
            }
        }
    }
    
    if(s==1){
        players[c.index] = ofSoundPlayer();
        players[c.index].loadSound(c.path);
        players[c.index].setPositionMS(c.begin*1000.0);
        players[c.index].play();
    }
    
    return false;
}




//bool AudioPlayer::Play(int uid,string path,float begin,float end ,ofParameter<float> & s){
//    
//    
//    if(players.size()>POLYPHONY && s>0)return false;
//    
//    
//    for(map<int,ofSoundPlayer>::iterator p= players.begin();p!=players.end();++p){
//        if(p->first==uid){
//            if(s ==1){
//                p->second.play();
//                s=0;
//                players.erase(p++);
//                
//                return false;
//            }
//            else if( s ==0){
//                p->second.stop();
//                players.erase(p++);
//                return false;
//                
//            }
//        }
//    }
//    
//    if(s==1){
//        players[uid] = ofSoundPlayer();
//        players[uid].loadSound(path);
//        players[uid].setPositionMS(begin*1000.0);
//        players[uid].play();
//    }
//    
//    return false;
//}
