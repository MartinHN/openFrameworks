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

#include "ofxTweener.h"






class Media : public GloveInteractBox{
    
    
    public :
    

    
    Media();
    virtual ~Media(){};
    
    
    MediaType type;
    string path;
    bool isLoaded=false;
    

    
    // for advanced drawing (translation and scale masked by current box)
    ofVec2f normalizedOffset;
    
    
    virtual void load(string filePath){};
    
    
    virtual void boxResized(bool stable);
    virtual void boxMoved(bool stable);
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
    
protected :
    
    //world position of drawing (parents box argument reffers to interactive zone, the drawing zone can be changed)
    ofRectangle drawBox;
    
    // world coordinate of clipped rectangle between drawBox and box 
    ofRectangle subSection;
    
};





#endif /* defined(__ImMedia__MediaReader__) */
