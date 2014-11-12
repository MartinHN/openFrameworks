//
//  MediaPresentation.h
//  ImMedia
//
//  Created by martin hermant on 12/11/14.
//
//

#ifndef __ImMedia__MediaPresentation__
#define __ImMedia__MediaPresentation__

#include "MediaPool.h"


class MediaPresentation : public Media{
    
public:
    
    MediaPresentation();
    ~MediaPresentation();
    
    
    
    
    vector<string> mediaPaths;
    ofParameter<int> curMediaIndex=0;
    Media* curMedia;
    
    
    virtual void load(string path);
    virtual void drawMedia();
    virtual void boxMoved();
    virtual void touch(){};
    void loadAt(int & idx);

    
    
};


#endif /* defined(__ImMedia__MediaPresentation__) */
