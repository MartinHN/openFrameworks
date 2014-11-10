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


void MediaVideo::update(){


    if(!loaded && player.isLoaded()){
        
        format = player.getWidth()*1.0/player.getHeight();

        loaded = true;
        player.setPaused(false);
        

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



void MediaVideo::touch(TouchType t,TouchAction a){
    GloveInteractBox::touch(t, a);
    if(t == GLOVE_CLICK){
        if(a==GLOVE_SHORTPRESS){
        if(!player.isPlaying())player.play();
        else player.setPaused(!player.isPaused());
        }
        else if ( a == GLOVE_LONGPRESS)
        {
            player.stop();
        }
    }
}
