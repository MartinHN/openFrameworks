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
#include "MediaPresentation.h"

#include "Screens.h"

#include "ofxXmlSettings.h"

class MediaPool{
    
public:
    
    MediaPool(){};
    ~MediaPool(){};
    
    
    static MediaPool * instance(){if(insti==NULL){insti = new MediaPool();}return insti;};

    vector<Media*> medias;
    
    
    static Media* createMedia(string filePath);
    bool loadMedias(string path);
    void unloadAll();
    void makeGrid(vector<Media*> _medias,ofRectangle rr);

    
    
    ofParameterGroup settings;
    
private:
    static MediaPool * insti;
    
};
#endif /* defined(__ImMedia__MediaPool__) */
