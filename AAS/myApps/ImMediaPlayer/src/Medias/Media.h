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
    bool isLoaded=false;
    
    
    Media(){
        isCollider = true;
    };
    virtual ~Media(){

};
    
    
    virtual void load(string filePath){};
    virtual void boxResized(){};
    virtual void boxMoved(){};
    void draw();
    virtual void update();
    
    
    // overide this in media class
    virtual void drawMedia(){};
    
    static ofImage loadingImage;
    static string loadingImagePath;
    
    ofRectangle loadingRect;
    

};





#endif /* defined(__ImMedia__MediaReader__) */
