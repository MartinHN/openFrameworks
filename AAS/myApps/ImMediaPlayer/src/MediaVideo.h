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



class MediaVideo: public Media {
    
public:
    MediaVideo(){};
    ~MediaVideo(){player.close();};
    
    ofVideoPlayer player;
    
    
    void load(string filePath);
    void unLoad();
    void draw(ofEventArgs & a);
    
    
    void setTime(float pct);

    void update(ofEventArgs & a);
    
    
private:
    bool loaded = false;
};
#endif /* defined(__ImMedia__MediaVideo__) */
