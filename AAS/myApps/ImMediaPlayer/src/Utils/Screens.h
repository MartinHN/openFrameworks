//
//  Screens.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

// class for handling all screens data

#ifndef __ImMedia__Screens__
#define __ImMedia__Screens__


#include "ofMain.h"
#include "MetaScreen.h"

#include "Config.h"


class Screens{
public:
    
    Screens();
    
    vector<MetaScreen*> screens;
    vector<MetaScreen*> walls;
    MetaScreen* full;
   
    ofVec2f resolution;

    static Screens* instance(){if(inst==NULL)inst = new Screens();
                                return inst;}
    
    private :
    static Screens * inst;
    ofRectangle getWall(int i);
};

#endif /* defined(__ImMedia__Screens__) */
