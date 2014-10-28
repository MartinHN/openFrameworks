//
//  MediaReader.cpp
//  ImMedia
//
//  Created by Tinmar on 26/10/2014.
//
//

#include "Media.h"


#include "MediaVideo.h"
#include "MediaImage.h"
#include "MediaMP3.h"
#include "MediaPdf.h"


Media * Media::createMedia(string filePath){
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




//void Media::zoom(float z);
//void Media::scroll(ofVec2f v);
