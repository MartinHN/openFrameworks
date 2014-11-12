//
//  MediaPresentation.cpp
//  ImMedia
//
//  Created by martin hermant on 12/11/14.
//
//

#include "MediaPresentation.h"


MediaPresentation::MediaPresentation(){
    curMediaIndex.addListener(this, &MediaPresentation::loadAt);
    
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
    isLoaded = true;
    
    
}


void MediaPresentation::loadAt(int & idx){
    // no recursivity thanks to ofParameter..
    curMediaIndex== idx%mediaPaths.size();
    
    if(curMedia!=NULL){
        delete curMedia;
    }
    if(curMediaIndex >=0 && curMediaIndex < mediaPaths.size())
        curMedia =  MediaPool::createMedia(mediaPaths[curMediaIndex]);
    
}

void MediaPresentation::drawMedia(){
    if(curMedia !=NULL){
        curMedia->draw();
    }
    
    
    
}

// smothing is handled by this containter so we set curMediabox directly
void MediaPresentation::boxMoved(){
    if(curMedia !=NULL){
        curMedia->box = box;
    }
}