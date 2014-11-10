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

void AudioPlayer::update(ofEventArgs & a){
  
    ofFmodSoundUpdate();
}

void AudioPlayer::Load(Container const & c,bool t){
audioUID id = getUID(c);
    if(t){
        players[id] = new ofFmodSoundPlayer();
        if(!players[id]->loadSound(c.path,true,c.begin,c.end));//ofLogError("can't load : "+id.name);
    }
    else{
    
    players[id]->stop();
        delete players[id];
    players.erase(id);
    }
    players[id]->setMultiPlay(true);
}

bool AudioPlayer::Play(Container & c, int s){
    audioUID id = getUID(c);
    
    
    map<audioUID,ofFmodSoundPlayer*>::iterator it = players.find(id);
    
    
    // play existing and playing

    if(it!=players.end()){
            //restart
            if(s ==1 && it->second!=NULL){
                
//                TimeLine.delDel("stop "+id.toString());

                cout << "restart playing " << id.toString() << "  " <<ofGetElapsedTimef() << endl;
                
                ofRemoveListener(ofFmodSoundPlayer::audioEvent,inst,&AudioPlayer::gotAudioEvent);
//                cout << ofGetElapsedTimef() << endl;
                it->second->play();
//                cout << ofGetElapsedTimef() << endl;
                it->second->setPositionMS(c.begin*1000.0);
//                cout << ofGetElapsedTimef() << endl;
                it->second->setStopMS((c.end-c.begin)*1000.0);
                ofEventArgs a  = ofEventArgs();
                instance()->update(a);
                cout << "end load playing " << ofGetElapsedTimef() << endl;
                ofAddListener(ofFmodSoundPlayer::audioEvent,inst,&AudioPlayer::gotAudioEvent);
                return true;
            }
            //stop it
            else if( s ==0){
                if(it->second)it->second->stop();
//                players.erase(p++);
                return false;
                
            }
        }
    
    
    // play already loaded

    for(map<audioUID,ofFmodSoundPlayer*>::iterator p= players.begin();p!=players.end();++p){
        
        // start preloaded
        if(s == 1 && p->first.name == id.name && p->second!=NULL && !p->second->getIsPlaying()){
            players[id] = p->second;
            ofRemoveListener(ofFmodSoundPlayer::audioEvent,inst,&AudioPlayer::gotAudioEvent);
            p->second->play();
            p->second->setPositionMS(c.begin*1000.0);
            p->second->setStopMS((c.end-c.begin)*1000.0);
            ofEventArgs a  = ofEventArgs();
            instance()->update(a);
            
            players.erase(p);
            ofAddListener(ofFmodSoundPlayer::audioEvent,inst,&AudioPlayer::gotAudioEvent);
            return true;
        }
    }

    
    // load on the go
    if(s==1){
        cout << "loadOntheGo " << ofGetElapsedTimef() << endl;

        Load(c,true);
        players[id]->play();
        players[id]->setPositionMS(c.begin*1000.0);
        players[id]->setStopMS((c.end-c.begin)*1000.0);

        return true;
    }
    
    return false;
}

void AudioPlayer::gotAudioEvent(std::pair<FMOD_CHANNEL*,FMOD_CHANNEL_CALLBACKTYPE> & ev){
    bool found = false;
    for(std::map<audioUID,ofFmodSoundPlayer*>::iterator it = players.begin() ; it != players.end() ; ++it){
        
        if(it->second->channel ==ev.first){
            if(ev.second==FMOD_CHANNEL_CALLBACKTYPE_END){
            Container::containers[it->first.idx].state = 0;
                found = true;
                cout << "stop " << endl;
            
            }
        }
    }
    if(!found){
        cout << "stop not found" << endl;
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
