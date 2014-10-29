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

//class Media;

#include "Media.h"
#include "ofQTKitPlayer.h"


class MediaVideo: public Media {
    
public:
    MediaVideo(){};
    ~MediaVideo(){};
    
    ofVideoPlayer player;
    
    
    void load(string filePath);
    void unLoad();
    void draw(ofEventArgs & a);
    
    
    void setTime(float pct);

    
};
#endif /* defined(__ImMedia__MediaVideo__) */
