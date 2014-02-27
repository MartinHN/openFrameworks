//
//  Player.h
//  RTaudio
//
//  Created by martin hermant on 27/02/14.
//
//

#ifndef __RTaudio__Player__
#define __RTaudio__Player__


#include "ofMain.h"
#include "Timed.h"

class Player{
public:
    
    Player();
    void play(float t,float til);
    void setup(){};
    void gotMessage(ofMessage & msg);
    void play(float t);
    void load(string f);
    
    vector<ofSoundPlayer> playerl;
};





#endif /* defined(__RTaudio__Player__) */
