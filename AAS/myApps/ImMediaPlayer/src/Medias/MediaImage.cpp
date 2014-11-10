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
    format = image.width*1.0/image.height;
}

void MediaImage::draw(){
    
    image.draw(box.x, box.y,box.width,box.height);

}


void MediaImage::resize(int x, int y){

}





