//
//  MediaPool.h
//  ImMedia
//
//  Created by Tinmar on 29/10/2014.
//
//

#ifndef __ImMedia__MediaPool__
#define __ImMedia__MediaPool__


#include <stdio.h>
#include "ofMain.h"
//#include "Media.h"
#include "MediaVideo.h"
#include "MediaImage.h"
#include "MediaMP3.h"
#include "MediaPdf.h"



class MediaPool{
    
public:
    static MediaPool * instance(){if(insti==NULL){insti = new MediaPool();}return insti;};

    vector<Media*> medias;
    
    
    Media* createMedia(string filePath);
    void loadMedia(string path);
    void unloadAll();
    
    
private:
    static MediaPool * insti;
    
};
#endif /* defined(__ImMedia__MediaPool__) */
