//
//  AudioPlayer.cpp
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#include "AudioPlayer.h"


std::map<audioUID,ofFmodSoundPlayer*>  AudioPlayer::players;
AudioPlayer * AudioPlayer::inst;



AudioPlayer::AudioPlayer(){
    
}

void AudioPlayer::Load(Container const & c,bool t){
audioUID id = getUID(c);
    if(t){
        players[id] = new ofFmodSoundPlayer();
        if(!players[id]->loadSound(c.path,true));//ofLogError("can't load : "+id.name);
    }
    else{
    
    players[id]->stop();
        delete players[id];
    players.erase(id);
    }
    players[id]->setMultiPlay(false);
}

bool AudioPlayer::Play(Container & c, int s){
    audioUID id = getUID(c);
    
//    if(players.size()>POLYPHONY && s>0)return false;
    
    int i = 0;
    for(map<audioUID,ofFmodSoundPlayer*>::iterator p= players.begin();p!=players.end();++p){
        
        //playing start slice
        if(p->first==id){
            //restart
            if(s ==1 && p->second!=NULL){
                
                TimeLine.delDel("stop "+id.toString());
                // underlying FMOD_play_sound seems faster to reload in streaming mode  (2ms Vs 20ms)
                cout << "restart playing " << ofGetElapsedTimef() << endl;
                players[id]->loadSound(c.path,true);
                
                p->second->play();
                p->second->setPositionMS(c.begin*1000.0);
                p->second->setStopMS((c.end-c.begin)*1000.0);
                
                cout << "end load playing " << ofGetElapsedTimef() << endl;
                TimeLine.addDel((c.end-c.begin)*1001.0f,"stop "+id.toString());
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
            cout << "start playing " << ofGetElapsedTimef() << endl;
            players[id]->play();
            players[id]->setPositionMS(c.begin*1000.0);
            p->second->setStopMS((c.end-c.begin)*1000.0);
            TimeLine.addDel((c.end-c.begin)*1001.0f,"stop "+id.toString());
            
            players.erase(p++);
            
            
            return true;
        }
        i++;
    }

    
    // load on the go
    if(s==1){
        players[id] = new ofFmodSoundPlayer();
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


void AudioPlayer::UnloadAll() {
    
    for(map<audioUID,ofFmodSoundPlayer*>::iterator it = players.begin() ; it!= players.end() ;++it){
        if(it->second!=NULL){
            it->second->unloadSound();
            delete it->second;
        }
        players.erase(it++);
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

