//
//  MediaVideo.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__MediaVideo__
#define __ImMedia__MediaVideo__

#include <stdio.h>

#include "Media.h"


class MediaVideo: public Media {
    
public:
    MediaVideo();
    ~MediaVideo();
    
    ofVideoPlayer player;
    
    
    void load(string filePath);
    void unLoad();
    void draw();
    
    
    void setTime(float pct);

    
};
#endif /* defined(__ImMedia__MediaVideo__) */
