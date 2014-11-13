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
    isLoaded = true;
}

void MediaImage::drawMedia(){
    

     image.drawSubsection(drawBox.x, drawBox.y,drawBox.width,drawBox.height,normalizedR.x*image.width,normalizedR.y*image.height,normalizedR.width*image.width,normalizedR.height*image.height);
}









