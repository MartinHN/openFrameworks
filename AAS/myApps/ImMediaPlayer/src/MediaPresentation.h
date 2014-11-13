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
    
    // double buffer for swapping easily
    typedef struct DoubleMediaBuffer{
        
        ~DoubleMediaBuffer(){
            if(front!=NULL)delete front;
            if(back!=NULL)delete back;
        };
        Media* front;
        Media* back;
        
        void swap(){
            Media* tmp = front;
            front = back;
            back = tmp;
        };
        
    }DoubleMediaBuffer;
    
    DoubleMediaBuffer curMedia;
    
    float transitionTime = 2;
    
    virtual void load(string path);
    virtual void drawMedia();
    virtual void boxMoved(bool stable);
    virtual void touch(TouchButton num, TouchAction s);
    void loadAt(int & idx);
    
    void startTransition();
    void updateTransition(const void * ended,float & a);
    
    float crossfade;
    ofEvent<float> crossfadeEv;
    
};


#endif /* defined(__ImMedia__MediaPresentation__) */
