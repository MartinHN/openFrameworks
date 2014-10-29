//
//  Screens.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__Screens__
#define __ImMedia__Screens__


#include "ofMain.h"
#include "GloveInteract.h"



class ScreenSpace : public ofRectangle{
    
    public :
    ScreenSpace();
    ScreenSpace(bool l){updw = l;};
    
    GloveInteract anchor;
    bool updw;
};




class Screens{
public:
    
    Screens();
    
    vector<ScreenSpace> screens;
    


    
    ofVec2f resolution;
    vector<ScreenSpace> walls;

    

    
    static Screens* instance(){if(inst==NULL)inst = new Screens();
                                return inst;}
    
    private :
    static Screens * inst;
    ofRectangle getWall(int i);
};

#endif /* defined(__ImMedia__Screens__) */
