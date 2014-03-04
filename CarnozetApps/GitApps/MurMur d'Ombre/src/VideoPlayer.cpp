//
//  VideoPlayer.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 04/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "VideoPlayer.h"



VideoPlayer::VideoPlayer(VisuHandler * v):VisuClass(v){
    MYPARAM(playPauseStop,0,0,3);
    playPauseStop.addListener(this, &VideoPlayer::ppp);
    playPauseStop.setSerializable(false);
    settings.setName("VideoPlayer");
    
    
    player.loadMovie("Video/mains.mp4");
    lasts=playPauseStop;
}


void VideoPlayer::update(int w, int h){

    player.update();
}


void VideoPlayer::ppp(int & s){
#ifndef GUIMODE
    if(s!=lasts){
    switch(s){
        case 1:{
            
            player.setPaused(!player.isPaused());
            lasts=s;
            break;
        }
        case 2:{
            player.play();
            lasts=s;
            break;
        }
        case 0:{
            player.stop();
            lasts=s;
            break;
        }
    }
    }
#endif
}
void VideoPlayer::draw(int w, int h){
    ofSetColor(255);
    if(player.isFrameNew()){
    player.draw(0,0,w,h);
    }
   }
