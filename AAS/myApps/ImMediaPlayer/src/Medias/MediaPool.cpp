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
    Media* m;
    for(vector<Media*>::iterator it = medias.begin() ; it!=medias.end() ; ++it){
        m = (*it);
        delete m;
    }
    medias.clear();
    
}

bool MediaPool::loadMedias(string path){
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
    if( medias.size()==0){
        return false;
    }
    
    Screens * ss = Screens::instance();
    ofRectangle rr = ss->walls[1]->rectScreen();
    makeGrid(medias,rr);
    
    return true;
}

void MediaPool::makeGrid(vector<Media*> _medias,ofRectangle rr){
    
    float ratio = rr.width/rr.height;
    
    int nstepx = sqrt(_medias.size())+1;
    int nstepy = nstepx;//rr.width*/
    ofVec2f step(rr.width*1.0/nstepx,rr.height*1.0/nstepy);
    int idxx = 0;
    ofRectangle tmp,originRect;
    for(vector<Media*>::iterator it=_medias.begin() ; it!=_medias.end() ; ++it){
        ofVec2f pos( ((int)idxx%nstepx)+0.51,((int)idxx/nstepx)+0.51);
        pos*=step;
        pos+= ofVec2f(rr.x,rr.y);
        tmp.setFromCenter(pos.x,pos.y,step.x*.8,step.y*.8);
        originRect.setFromCenter(pos.x,pos.y,step.x*.2,step.y*.2);
        (*it)->box = originRect;
        (*it)->targetBox = tmp;
        (*it)->updateDrawBox();
        
        idxx++;
    }
    
}

Media * MediaPool::createMedia(string filePath){
    ofFile file(filePath);
    string ext = file.getExtension();
    Media * m = NULL;
    int foundIdx=-1;

    
    if(ext == "avi" || ext == "mp4" || ext == "mov"){
        m = (Media*) new MediaVideo();
    }
    else if(ext == "jpeg" || ext == "jpg" || ext == "png" || ext == "gif"){
        m = (Media*) new MediaImage();
    }
    
    else if(ext == "pdf" ){
        m = (Media*) new MediaPdf();
    }
    
    else if(ext == "mp3" || ext == "wav"){
        m = (Media*) new MediaMP3();
    }
    
    else if((foundIdx = file.getFileName().find_last_of("_presentation") )!= string::npos && foundIdx == file.getFileName().length() - 13){
        m = (Media*) new MediaPresentation();
    }
    
    if(m==NULL)return NULL;
    
    
    
    m->name = file.getBaseName();
    m->path = filePath;
    m->load(filePath);
    
    file.close();
    return m;
};




