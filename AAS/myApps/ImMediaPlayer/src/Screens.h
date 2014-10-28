//
//  Screens.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__Screens__
#define __ImMedia__Screens__

#include <stdio.h>
#include "ofMain.h"




class Screens{
public:
    
    Screens();
    
    vector<ofRectangle> screens;
    
    ofVec2f totalRes();
    ofRectangle getWall(int i);
    
    
    
    
    
    static Screens* instance(){if(inst==NULL)inst = new Screens();
                                return inst;}
    
    private :
    static Screens * inst;
    
};

#endif /* defined(__ImMedia__Screens__) */
