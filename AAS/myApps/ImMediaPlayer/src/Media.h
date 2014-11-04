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
#include "ofxUIImageButton.h"






class Media : public GloveInteract,ofxUIImageButton{
    
    
    public :


    MediaType type;
    string path;

    
    
    Media(){
        ofxUIImageButton();
        isCollider = true;
    };
    ~Media(){

};
    
    
    virtual void load(string filePath){};
    virtual void unLoad(){};
    virtual void draw(){};


};





#endif /* defined(__ImMedia__MediaReader__) */
