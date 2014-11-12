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
    
    image.draw(drawBox.x, drawBox.y,drawBox.width,drawBox.height);

}







