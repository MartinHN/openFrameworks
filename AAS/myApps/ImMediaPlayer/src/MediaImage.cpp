//
//  MediaImage.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaImage.h"


void MediaImage::load(string filePath){
    
    image.loadImage(filePath);
}

void MediaImage::draw(ofEventArgs & a){
    image.draw(box);
}



void MediaImage::unLoad(){
    image.clear();
}


