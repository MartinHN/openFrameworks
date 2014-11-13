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

//#include "ofxTweener.h"


class MediaVideo: public Media {
    
public:
    MediaVideo(){Media::Media();type = VIDEO;};
    ~MediaVideo(){playImage.clear();pauseImage.clear();stopImage.clear();player.close();};
    
    ofVideoPlayer player;
    
    
    virtual void load(string filePath);
    virtual void drawMedia();
    virtual void update();
    virtual void boxMoved(bool stable);
    virtual void boxResized(bool stable);
    
    virtual void touch(TouchButton t,TouchAction a);
    
    void setTime(float pct);
    
    ofImage playImage;
    float alphaPlay;
    ofImage pauseImage;
    float alphaPause;
    ofImage stopImage;
    float alphaStop;
    
    ofRectangle logoRect;
    
private:
    
};
#endif /* defined(__ImMedia__MediaVideo__) */
