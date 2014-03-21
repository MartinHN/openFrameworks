//
//  Player.cpp
//  RTaudio
//
//  Created by martin hermant on 27/02/14.
//
//

#include "Player.h"
int Player::number = 0;

Player::Player(){
    for (int i = 0 ; i< 4 ; i++){
    playerl.push_back(ofSoundPlayer());
    }
    ofAddListener(ofEvents().messageEvent,this,&Player::gotMessage);
    pid = number;
    number++;
}

Player::~Player(){
    number--;
}


void Player::gotMessage(ofMessage & msg){
    
    vector<string> inst = ofSplitString(msg.message," ");
    if (inst.size()>2){
    if(inst[0] == "p" && ofToInt(inst[1]) == pid){
        
        
        if(inst[2] == "play"){
            play( ofToFloat(inst[3]));
        }
        //    else if(inst[0] == "stop"){
        //        player.stop();
        //    }
        
        else if(inst[2]=="stop4"){
            playerl[ofToInt(inst[3])].stop();
        }
        
        else if(inst[2] == "play4"){
            play( ofToFloat(inst[3]),ofToFloat(inst[2]));
        }
        

    
    }
        
    }
    

    

}


void Player::load(string f){
    if(f!=""){
    for(vector<ofSoundPlayer>::iterator player = playerl.begin() ; player!=playerl.end() ; ++player){
       player->loadSound(f);
    
        }
    }
    
}



void Player::play(float t,float til){
    
    for (int i = 0 ; i < playerl.size() ; i++){
        if (!playerl[i].getIsPlaying()){
    
            printf("play at %f\n",t);
            playerl[i].play();
            playerl[i].setPositionMS(1000.0*t);
            TimeLine.addDel(til*100000.0, "p "+ofToString(pid)+" stop4 "+ofToString(i));
            break;
        }}
    
    //    if(player.)
}

void Player::play(float t){
    
//    for (int i = 0 ; i < playerl.size() ; i++){
//    if (player.getIsPlaying()) player.stop();
//    
//        printf("play at %f\n",t);
//        player.play();
//        player.setPositionMS(1000.0*t);
//    }
    
//    if(player.)
}