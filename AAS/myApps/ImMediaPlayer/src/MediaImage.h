//
//  MediaImage.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__MediaImage__
#define __ImMedia__MediaImage__

#include <stdio.h>
#include "Media.h"



class MediaImage : public Media{
    
    
public:
    MediaImage(){};
    ~MediaImage(){};
   
    
    ofImage image;
   
   void load(string filePath);
   void unLoad();
   void draw(ofEventArgs & a);
};
#endif /* defined(__ImMedia__MediaImage__) */
