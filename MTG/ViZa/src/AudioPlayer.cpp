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
    
    int i = 0;
    for(map<int,ofSoundPlayer>::iterator p= players.begin();p!=players.end();++p){
        if(p->first==c.index){
            if(s ==1){
                players[c.index].setPositionMS(c.begin*1000.0);
                TimeLine.addDel((c.end-c.begin)*1000.0f,"stop "+ofToString(c.index));
//                s=0;
//                players.erase(p++);
                
                return true;
            }
            else if( s ==0){
                p->second.stop();
                players.erase(p++);
                return false;
                
            }
        }
        i++;
    }
    
    if(s==1){
        players[c.index] = ofSoundPlayer();
        players[c.index].loadSound(c.path);
        
        players[c.index].play();
        players[c.index].setPositionMS(c.begin*1000.0);
        TimeLine.addDel((c.end-c.begin)*1000.0f,"stop "+ofToString(c.index));
    }
    
    return false;
}

void AudioPlayer::gotMessage(ofMessage & msg){
    vector<string> inst = ofSplitString(msg.message," ");
    if (inst.size()>=2){
        if(inst[0]=="stop"){
            players[ofToInt(inst[1])].stop();
            Container::containers[ofToInt(inst[1])].state = 0;
        }
    }
    
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
