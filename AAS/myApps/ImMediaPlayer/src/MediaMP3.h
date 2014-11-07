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
    ~MediaMP3(){};
    
  
    virtual void load(string filePath){};
    virtual void draw(){};
    virtual void resize(int x, int y){};
};


#endif /* defined(__ImMedia__MediaMP3__) */
