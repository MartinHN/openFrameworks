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
    
    
    box.set(0,0,600,100);//player.getWidth(),player.getHeight());
}

void MediaVideo::unLoad(){
    
    player.close();
}
void MediaVideo::update(ofEventArgs & a){
    GloveInteract::update(a);
    player.update();
}

void MediaVideo::draw(ofEventArgs & a){
    ofPushMatrix();
    ofTranslate(box.x, box.y);
    ofScale(box.width*1.0/player.width,box.height*1.0/player.height);
   if(player.isLoaded()) player.draw(0,0);
    ofPopMatrix();
//        player.draw(box);
}



void MediaVideo::setTime(float pct){
    player.setPosition(pct);
}
