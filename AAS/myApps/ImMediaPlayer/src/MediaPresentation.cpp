//
//  MediaPresentation.cpp
//  ImMedia
//
//  Created by martin hermant on 12/11/14.
//
//

#include "MediaPresentation.h"


MediaPresentation::MediaPresentation(){
    Media::Media();
    GloveInteractBox::GloveInteractBox();
    curMediaIndex.addListener(this, &MediaPresentation::loadAt);

    
    crossfade.addListener(this,&MediaPresentation::updateTransition);
    curMedia.front = NULL;
    curMedia.back = NULL;
    type = PRESENTATION;
}

MediaPresentation::~MediaPresentation(){

    
}


void MediaPresentation::load(string folderPath){
    if(!ofDirectory::doesDirectoryExist(folderPath))return;
    
    ofDirectory d(folderPath);
    
    d.listDir();
    vector<ofFile> files  = d.getFiles();
    
    for(vector<ofFile>::iterator it = files.begin() ; it !=files.end() ; ++it){
        if(ofFind(supported_formats, it->getExtension())){
            mediaPaths.push_back(it->path());
        }
    }
    
    curMediaIndex = 0;
    curMedia.swap();
    isLoaded = true;
    
    
}


void MediaPresentation::clicked(TouchButton & num){
    if(wasSelected){
    if(num == GLOVE_BUTTON_CLICK){
        curMediaIndex++;
        startTransition();
    }
    }
    

    
    
    
}

void MediaPresentation::loadAt(int & idx){
    // no recursivity thanks to ofParameter..
    curMediaIndex= idx%mediaPaths.size();
    
    // if tween not ended
    if(crossfade!=0){
        curMedia.swap();
    }
    if(curMedia.back!=NULL){
        delete curMedia.back;
    }
    
    curMedia.back =  MediaPool::createMedia(mediaPaths[curMediaIndex]);
    ofRectangle tmpR(0,0,1,1);
    tmpR.scaleTo(box);
    curMedia.back->box = tmpR;
    curMedia.back->targetBox = tmpR;
    curMedia.back->normalizedOffset.set(1,0);
    curMedia.back->updateDrawBox();
    
    curMedia.back->isDraggable = false;
    curMedia.back->isSelectable = false;
    curMedia.back->isCollider = false;
    curMedia.back->drawBaseFeedBack = false;


    ofRemoveListener(drawSyphonEvent, (GloveInteractBox*)curMedia.back, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+curMedia.back->drawLayer);
    
}

void MediaPresentation::startTransition(){
    crossfade = 1;
    Tweener.addTween(&crossfade, 0.0f, transitionTime);
}

void MediaPresentation::drawMedia(){

    if(curMedia.back !=NULL){
        ofPushStyle();
        curMedia.back->drawMedia();
        ofPopStyle();
    }
    if(curMedia.front !=NULL){
        ofPushStyle();
        curMedia.front->drawMedia();
        ofPopStyle();
    }

    
    
}

// smothing is handled by this containter so we set Mediabox directly
void MediaPresentation::boxChanged(bool stable){
    if(curMedia.front !=NULL){
        curMedia.front->box = box;
        curMedia.front->targetBox = box;
        curMedia.front->updateDrawBox();
        curMedia.front->boxMoved(stable);
    }
    if(curMedia.back !=NULL){
        curMedia.back->box = box;
        curMedia.back->targetBox = box;
        curMedia.back->updateDrawBox();
        curMedia.back->boxMoved(stable);
    }
}

void MediaPresentation::updateTransition(float & a){
    
    if(a==0){
        curMedia.swap();
        delete curMedia.back;
        curMedia.back = NULL;
    }
    else{
        if(curMedia.front !=NULL){
            curMedia.front->normalizedOffset.set(a-1,0);
            curMedia.front->updateDrawBox();
        }
        
        if(curMedia.back !=NULL){
            curMedia.back->normalizedOffset.set(a,0);
            curMedia.back->updateDrawBox();
        }
        
    }
}