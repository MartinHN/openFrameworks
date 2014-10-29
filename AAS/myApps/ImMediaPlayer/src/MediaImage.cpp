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


    image.width = box.width;
    image.height = box.height;

    image.draw(box.x-image.width/2.0, box.y-image.height/2.0);

}



void MediaImage::unLoad(){
    image.clear();
}


