//
//  MediaPool.cpp
//  ImMedia
//
//  Created by Tinmar on 29/10/2014.
//
//




#include "MediaPool.h"





//class MediaVideo;
//class MediaImage;
//class MediaMP3;
//class MediaPdf;


MediaPool* MediaPool::insti;

void MediaPool::unloadAll(){
    bool last = false;
    for(vector<Media*>::iterator it = medias.begin() ; it!=medias.end() ; ++it){
        delete (*it);
        medias.erase(it++);
        if(medias.size() == 0)break;
        
    }
    
}

void MediaPool::loadMedia(string path){
    unloadAll();
    ofDirectory dir(path);
    dir.listDir();
    vector<ofFile> mediasf = dir.getFiles();
    for(vector<ofFile>::iterator it=mediasf.begin() ; it!=mediasf.end() ; ++it){
        Media * m =createMedia(it->path());
        if(m!=NULL){
            medias.push_back(m);
        }
    }
    
}

Media * MediaPool::createMedia(string filePath){
    ofFile file(filePath);
    string ext = file.getExtension();
    Media * m = NULL;
    
    
    if(ext == "avi" || ext == "mp4" || ext == "mov"){
        m = (Media*) new MediaVideo();
    }
    else if(ext == "jpeg" || ext == "jpg" || ext == "png"){
        m = (Media*) new MediaImage();
    }
    
    else if(ext == "pdf" ){
        m = (Media*) new MediaPdf();
    }
    
    else if(ext == "mp3" || ext == "wav"){
        m = (Media*) new MediaMP3();
    }
    
    
    file.close();
    
    m->name = file.getBaseName();
    m->path = filePath;
    m->load(filePath);
    
    return m;
};