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
    ofPushMatrix();
    ofTranslate(box.x, box.y);
    image.resize(box.width, box.height);
    image.draw(0,0);
    ofPopMatrix();
}



void MediaImage::unLoad(){
    image.clear();
}


