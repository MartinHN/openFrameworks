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

    player.setLoopState(OF_LOOP_NORMAL);
    player.play();
    
    

}

void MediaVideo::unLoad(){
    
    player.close();
}
void MediaVideo::update(ofEventArgs & a){

    GloveInteract::update(a);
    if(!loaded && player.isLoaded()){
        box.set(0,0,player.getWidth(),player.getHeight());
        loaded = true;
    }
    player.update();
}

void MediaVideo::draw(ofEventArgs & a){
    if(loaded){

        player.width = box.width;
        player.height = box.height;
    player.draw(box.x,box.y);

    }
//        player.draw(box);
}



void MediaVideo::setTime(float pct){
    player.setPosition(pct);
}
