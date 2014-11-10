//
//  MediaMP3.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__MediaMP3__
#define __ImMedia__MediaMP3__

#include <stdio.h>
#include "Media.h"


class MediaMP3 :public  Media{
    
    
public:
    MediaMP3(){};
    ~MediaMP3(){player.unloadSound();};
    
  
    virtual void load(string filePath){player.loadSound(filePath);};
    virtual void draw(){};
    virtual void resize(int x, int y){};
    
    
    ofSoundPlayer player;
};


#endif /* defined(__ImMedia__MediaMP3__) */
