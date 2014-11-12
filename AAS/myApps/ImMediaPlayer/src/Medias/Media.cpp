//
//  Media.cpp
//  ImMedia
//
//  Created by Tinmar on 29/10/2014.
//
//

#include <stdio.h>
#include "Media.h"

ofImage Media::loadingImage;
string Media::loadingImagePath = "Medias/loading.gif";


void Media::draw(){
    if(isLoaded)this->drawMedia();
    else {
        if(!loadingImage.isAllocated()){
            loadingImage.loadImage(loadingImagePath);

        }
        
        loadingRect.set(box.getCenter(),loadingImage.width,loadingImage.height);
        loadingRect.scaleTo(box, OF_SCALEMODE_FIT);
        loadingImage.draw(loadingRect);
    }
}

void Media::update(){
    if(!loadingImage.isAllocated()){
        loadingImage.loadImage(loadingImagePath);
    }
}