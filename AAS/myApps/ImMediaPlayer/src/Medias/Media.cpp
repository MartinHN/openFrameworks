//
//  Media.cpp
//  ImMedia
//
//  Created by Tinmar on 29/10/2014.
//
//

#include <stdio.h>
#include "Media.h"
#include "Screens.h"


ofImage Media::loadingImage;
string Media::loadingImagePath = "Medias/loading.gif";

Media::Media(){
    
    drawBox= box;
    subSection.set(0,0,1,1);
    normalizedOffset.set(0,0);
    isCollider = true;
    
}
void Media::draw(){
    if(isLoaded){
        ofPushMatrix();

        if(normalizedOffset.x<1 && normalizedOffset.y < 1){
        this->drawMedia();
        }
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

void Media::boxResized(bool stable){
    updateDrawBox();
    if(stable && !Screens::instance()->isFullscreened(this))checkBoxFormat();
};

void Media::boxMoved(bool stable){
    updateDrawBox();
    if(stable && !Screens::instance()->isFullscreened(this))checkBoxFormat();
};


void Media::updateDrawBox(){
    // reinit drawBox
    drawBox.setFromCenter(box.getCenter(), format,1 );
    drawBox.scaleTo(box,OF_SCALEMODE_FIT);
    drawBox=drawBox+normalizedOffset*ofVec2f(drawBox.width,drawBox.height);
    
    // compute subsection
    subSection = box.getIntersection(drawBox);

}

// reformat when dezooming
void Media::checkBoxFormat(){
    ofVec2f reformatZoom(0);
    // if were stable
    if(targetBox.width == box.width && targetBox.height == box.height){
        reformatZoom.x = MAX(targetBox.width-drawBox.width,0);
        reformatZoom.y = MAX(targetBox.height-drawBox.height,0);
        ofRectangle newR;

        newR.setFromCenter(targetBox.getCenter(),targetBox.width-reformatZoom.x,targetBox.height-reformatZoom.y);
        
        makeValid(newR);
//    drawBox.scaleTo(targetBox);
//    targetBox.scaleTo(drawBox);
    
    
        targetBox.set(newR);
    }

}


