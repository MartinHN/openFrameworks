//
//  MediaVideo.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaVideo.h"


void MediaVideo::load(string filePath){
//    player.setPixelFormat(OF_PIXELS_RGB);
    bool Loaded = player.loadMovie((filePath));
    player.setPaused(false);
    player.setLoopState(OF_LOOP_NORMAL);
    box.set(0,0,100,100);//player.getWidth(),player.getHeight());
}

void MediaVideo::unLoad(){
    
    player.close();
}


void MediaVideo::draw(ofEventArgs & a){
    player.update();
    if(player.isLoaded()){
//        if(player.isPaused()||!player.isPlaying()){
         player.play();
//        }
        player.nextFrame();
        player.draw(box);
    }

//    player.draw(box);
}



void MediaVideo::setTime(float pct){
    player.setPosition(pct);
}
