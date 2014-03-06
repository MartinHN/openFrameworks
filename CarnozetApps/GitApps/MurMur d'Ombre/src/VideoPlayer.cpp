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
            player.loadMovie("Video/mains.mp4");
            player.play();
            lasts=s;
            break;
        }
        case 0:{
            player.stop();
            player.close();
            lasts=s;
            break;
        }
    }
    }
#endif
}
void VideoPlayer::draw(int w, int h){
    if(playPauseStop == 0){
        ofSetColor(0);
    }
    else{
    ofSetColor(color->x,color->y,color->z,alpha);
    
    player.draw(0,0,w,h);
    }
   }
