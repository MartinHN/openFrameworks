//
//  MediaReader.h
//  ImMedia
//
//  Created by Tinmar on 26/10/2014.
//
//

#ifndef __ImMedia__Media__
#define __ImMedia__Media__

#include <stdio.h>
#include "GloveInteract.h"
#include "ofMain.h"





typedef enum{PDF,IMAGE,VIDEO} MediaType;


class Media : public GloveInteract{
    
    
    public :
    static Media* createMedia(string filePath);
    
    
    string name;
    MediaType type;
    string path;

    
    
    Media(){};
    ~Media(){};
    
    
    virtual void load(string filePath){};
    virtual void unLoad(){};
    virtual void draw(){};


};





#endif /* defined(__ImMedia__MediaReader__) */
