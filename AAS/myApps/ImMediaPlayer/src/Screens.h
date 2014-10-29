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
//#include "GloveInteract.h"
#include "GButton.h"



class ScreenSpace : public ofRectangle{
    
    public :
    ScreenSpace(){};
    ~ScreenSpace(){};
    ScreenSpace(ofRectangle r,bool l=false){
        set(r);
        anchor.isDraggable = false;
        anchor.box.set(getCenter().x - ANCHOR_WIDTH/2,l?this->height:0,  ANCHOR_WIDTH,ANCHOR_HEIGHT );
    }
    GButton anchor;

};




class Screens{
public:
    
    Screens();
    
    vector<ScreenSpace> screens;
    vector<ScreenSpace> walls;
    
    ScreenSpace full;


    
    ofVec2f resolution;


    

    
    static Screens* instance(){if(inst==NULL)inst = new Screens();
                                return inst;}
    
    private :
    static Screens * inst;
    ofRectangle getWall(int i);
};

#endif /* defined(__ImMedia__Screens__) */
