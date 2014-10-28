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
    MediaMP3();
    ~MediaMP3();
    
  
    void load(string filePath){};
    void unLoad(){};
   void draw(){};
};


#endif /* defined(__ImMedia__MediaMP3__) */
