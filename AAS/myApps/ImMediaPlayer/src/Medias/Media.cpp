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

Media::Media(){
    
    drawBox= box;
    normalizedR.set(0,0,1,1);
    normalizedOffset.set(0,0);
    isCollider = true;
    
}
void Media::draw(){
    if(isLoaded){
        ofPushMatrix();
        ofTranslate(normalizedOffset*ofVec2f(drawBox.x,drawBox.y));

        this->drawMedia();

        ofPopMatrix();
        
    }
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

void Media::boxResized(){
    updateDrawBox();
    checkBoxFormat();
};

void Media::boxMoved(){
    updateDrawBox();
};


void Media::updateDrawBox(){
    drawBox.scaleTo(box,OF_SCALEMODE_FIT);
    drawBox=drawBox+normalizedOffset*ofVec2f(drawBox.x,drawBox.y);
}

// reformat when dezooming
void Media::checkBoxFormat(){
    ofVec2f reformatZoom(0);
    if(targetBox.width < box.width || targetBox.height < box.height){
        reformatZoom.x = MAX(box.width-drawBox.width,0);
        reformatZoom.y = MAX(box.height-drawBox.height,0);
        ofRectangle newR;
        newR.setFromCenter(box.getCenter(),box.width-reformatZoom.x,box.height-reformatZoom.y);
        
        makeValid(newR);
        targetBox.set(newR);
    }
    
}


