//
//  MediaImage.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaImage.h"


void MediaImage::load(string filePath){
    
    image.load(filePath);
    format = image.width*1.0/image.height;
    isLoaded = true;
    updateDrawBox();
}

void MediaImage::drawMedia(){

    ofVec2f ratio = ofVec2f(image.width/drawBox.width,image.height/drawBox.height);
    image.getTexture().drawSubsection(subSection.x, subSection.y,subSection.width, subSection.height,(subSection.x - drawBox.x)*ratio.x,(subSection.y - drawBox.y)*ratio.y,subSection.width*ratio.x,subSection.height*ratio.y);
}









