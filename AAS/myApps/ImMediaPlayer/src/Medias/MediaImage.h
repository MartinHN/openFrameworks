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
    ~MediaImage(){image.clear();};
   
    
    ofImage image;
   
    virtual void load(string filePath);
    virtual void drawMedia();
    


};
#endif /* defined(__ImMedia__MediaImage__) */
