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
    
    
    ofAddListener(crossfadeEv,this,&MediaPresentation::updateTransition);
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


void MediaPresentation::touch(TouchButton num, TouchAction s){
    GloveInteractBox::touch(num, s);
    if(hovered[curGlove]){
    if(num == GLOVE_BUTTON_CLICK && s == GLOVE_ACTION_SHORTPRESS){
        curMediaIndex++;
        startTransition();
    }
    }
    
    if(crossfade ==0){
        //        curMedia.front->touch(num, s);
    }
    
    
    
}

void MediaPresentation::loadAt(int & idx){
    // no recursivity thanks to ofParameter..
    curMediaIndex= idx%mediaPaths.size();
    
    // if tween not ended
    if(crossfade>0){
        curMedia.swap();
    }
    if(curMedia.back!=NULL){
        delete curMedia.back;
    }
    
    curMedia.back =  MediaPool::createMedia(mediaPaths[curMediaIndex]);
    curMedia.back->box = box;
    curMedia.back->targetBox = box;
    curMedia.back->normalizedOffset.set(1,0);
    curMedia.back->updateDrawBox();
    
    curMedia.back->isDraggable = false;
    curMedia.back->isSelectable = false;
    curMedia.back->isCollider = false;
    curMedia.back->drawBaseFeedBack = false;
    curMedia.back->isHoverable = false;

    ofRemoveListener(drawSyphonEvent, (GloveInteractBox*)curMedia.back, &GloveInteractBox::draw,OF_EVENT_ORDER_BEFORE_APP+curMedia.back->drawLayer);
    
}

void MediaPresentation::startTransition(){
    crossfade = 1;
    Tweener.addTween(crossfade, 0.0f, transitionTime,&crossfadeEv,true);
}

void MediaPresentation::drawMedia(){
    if(curMedia.back !=NULL){
        curMedia.back->drawMedia();
    }
    if(curMedia.front !=NULL){
        curMedia.front->drawMedia();
    }
    
    
    
}

// smothing is handled by this containter so we set Mediabox directly
void MediaPresentation::boxMoved(bool stable){
    if(curMedia.front !=NULL){
        curMedia.front->box = box;
        curMedia.front->targetBox = box;
        curMedia.front->boxMoved(stable);
    }
    if(curMedia.back !=NULL){
        curMedia.back->box = box;
        curMedia.back->targetBox = box;
        curMedia.back->boxMoved(stable);
    }
}

void MediaPresentation::updateTransition(const void * ended,float & a){
    //hack in ofxTweener for knowing last event
    if(ended == NULL){
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