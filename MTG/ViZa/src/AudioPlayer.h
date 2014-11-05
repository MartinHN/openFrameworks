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

#include "Timed.h"
#include "Container.h"
class Container;


#define POLYPHONY 2

typedef struct audioUID{
    string name;
    int idx;
    
    bool  operator==(const audioUID &o) const{
        return idx == o.idx && name == o.name;
    }
    
    bool  operator<(const audioUID &o) const{
        return idx < o.idx ;
    }
    
    string toString() {
        return  name + "\n" + ofToString(idx) ;
    }
    
    friend class AudioPlayer ;
    
}audioUID;




class AudioPlayer{
public:
    AudioPlayer();
    
    static AudioPlayer * instance(){
        if(inst==NULL){inst = new AudioPlayer();
        ofAddListener(ofEvents().messageEvent,inst,&AudioPlayer::gotMessage);
        }return inst;
    };
    
    static std::map<audioUID,ofFmodSoundPlayer*> players;
    static bool Play(Container & c,int s);
    static void Load(Container const & c,bool t);
    static void UnloadAll();
//    static bool Play(int uid,string path,float begin,float end ,ofParameter<float> & s);
    
    void gotMessage(ofMessage & msg);
    static AudioPlayer * inst;
    
    static audioUID getUID(Container const & c);
    static audioUID audioUIDfromString(const string s);
    
};




#endif /* defined(__ViZa__AudioPlayer__) */
