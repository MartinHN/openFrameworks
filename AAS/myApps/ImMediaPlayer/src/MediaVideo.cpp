//
//  MediaVideo.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaVideo.h"


void MediaVideo::load(string filePath){
    
    player.loadMovie(filePath);
}

void MediaVideo::unLoad(){
    
    player.closeMovie();
}


void MediaVideo::draw(){
    player.draw(box);
}



void MediaVideo::setTime(float pct){
    player.setPosition(pct);
}
