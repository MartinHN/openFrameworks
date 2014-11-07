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

#include "GloveInteractBox.h"







class Media : public GloveInteractBox{
    
    
    public :


    MediaType type;
    string path;
    
    
    
    Media(){
        isCollider = true;
    };
    virtual ~Media(){

};
    
    
    virtual void load(string filePath){};
    virtual void resize(int x, int y){};
    virtual void draw(){};
    virtual void update(){};

    

};





#endif /* defined(__ImMedia__MediaReader__) */
