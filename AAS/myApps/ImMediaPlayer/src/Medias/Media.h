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
    

    
    Media();
    virtual ~Media(){};
    
    
    MediaType type;
    string path;
    bool isLoaded=false;
    
    
    ofRectangle drawBox;
    
    // for advanced drawing (translation and scale masked by current box)
    ofRectangle normalizedR;
    ofVec2f normalizedOffset;
    
    virtual void load(string filePath){};
    
    
    virtual void boxResized();
    virtual void boxMoved();
    void draw();
    
    
    virtual void update();
    void updateDrawBox();
    void checkBoxFormat();
    
    // overide this in media class
    virtual void drawMedia(){};

    
    
    
    // gif when loading
    static ofImage loadingImage;
    static string loadingImagePath;
    ofRectangle loadingRect;
    
    
};





#endif /* defined(__ImMedia__MediaReader__) */
