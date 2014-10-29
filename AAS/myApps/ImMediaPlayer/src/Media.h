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
#include "ofMain.h"

#include "GloveInteract.h"







class Media : public GloveInteract{
    
    
    public :

    static vector<Media*> allElements;
    MediaType type;
    string path;

    
    
    Media(){
        allElements.push_back(this);
    };
    ~Media(){
        vector<Media*>::iterator it =find(allElements.begin(),allElements.end(),this);
        if(it!=allElements.end())allElements.erase(it);
};
    
    
    virtual void load(string filePath){};
    virtual void unLoad(){};
    virtual void draw(){};


};





#endif /* defined(__ImMedia__MediaReader__) */
