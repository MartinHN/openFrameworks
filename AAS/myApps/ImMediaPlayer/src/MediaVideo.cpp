//
//  MediaVideo.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaVideo.h"


void MediaVideo::load(string filePath){

    bool Loaded = player.loadMovie(filePath);
    player.setLoopState(OF_LOOP_NONE);
    
    
//    player.play();
    
    

}

void MediaVideo::unLoad(){
    
    player.close();
}
void MediaVideo::update(){


    if(!loaded && player.isLoaded()){
        box.set(0,0,player.getWidth(),player.getHeight());
        format = player.getWidth()*1.0/player.getHeight();
        loaded = true;
    }
    player.update();
}

void MediaVideo::draw(){
    if(loaded){
    player.draw(box.x,box.y,box.width,box.height);

    }

}


void MediaVideo::resize(int x, int y){

}


void MediaVideo::setTime(float pct){
    player.setPosition(pct);
}
