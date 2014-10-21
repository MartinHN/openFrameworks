//
//  AudioPlayer.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "AudioPlayer.h"


std::map<audioUID,ofSoundPlayer*>  AudioPlayer::players;
AudioPlayer * AudioPlayer::inst;



AudioPlayer::AudioPlayer(){
    
}

void AudioPlayer::Load(Container const & c,bool t){
audioUID id = getUID(c);
    if(t){
        players[id] = new ofSoundPlayer();
        if(!players[id]->loadSound(c.path))ofLogError("can't load : "+id.name);
    }
    else{
    
    players[id]->stop();
        delete players[id];
    players.erase(id);
    }
}

bool AudioPlayer::Play(Container & c, int s){
    audioUID id = getUID(c);
    
//    if(players.size()>POLYPHONY && s>0)return false;

    int i = 0;
    for(map<audioUID,ofSoundPlayer*>::iterator p= players.begin();p!=players.end();++p){
        
        //playing start slice
        if(p->first==id){
            //restart
            if(s ==1 && p->second!=NULL){
                p->second->play();
                p->second->setPositionMS(c.begin*1000.0);
                TimeLine.addDel((c.end-c.begin)*1000.0f,"stop "+id.toString());
                return true;
            }
            //stop it
            else if( s ==0){
                if(p->second)p->second->stop();
//                players.erase(p++);
                return false;
                
            }
        }
        
        // start preloaded
        else if(s == 1 && p->first.name == id.name && p->second && !p->second->getIsPlaying()){
            players[id] = p->second;
            players[id]->play();
            players[id]->setPositionMS(c.begin*1000.0);
            TimeLine.addDel((c.end-c.begin)*1000.0f,"stop "+id.toString());
            
            players.erase(p++);
            
            
            return true;
        }
        i++;
    }

    
    // load on the go
    if(s==1){
        players[id] = new ofSoundPlayer();
        players[id]->loadSound(c.path);
        
        players[id]->play();
        players[id]->setPositionMS(c.begin*1000.0);
        TimeLine.addDel((c.end-c.begin)*1000.0f,"stop "+id.toString());
        return true;
    }
    
    return false;
}

void AudioPlayer::gotMessage(ofMessage & msg){
    vector<string> inst = ofSplitString(msg.message," ");
    if (inst.size()>=2){
        if(inst[0]=="stop"){
            inst.erase(inst.begin());
            string s  = ofJoinString(inst, " ");
            if(players[audioUIDfromString(s)]!=NULL)players[audioUIDfromString(s)]->stop();
            Container::containers[audioUIDfromString(s).idx].state = 0;
        }
    }
    
}

            
audioUID AudioPlayer::getUID(Container const & c){
    audioUID id;
    id.idx= c.index;
    id.name = c.filename;
    return id;
}


audioUID AudioPlayer::audioUIDfromString(const string s){
    audioUID res;
    vector<string> ss = ofSplitString(s,"\n");
    res.name = ss[0];//ofJoinString(s[0], " ");
    res.idx = ofFromString<int>(ss[1]);
    return res;
}

