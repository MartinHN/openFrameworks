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
//#include "GButton.h"
#include "Config.h"
//#include "ofxUI.h"


class MetaScreen : public ofRectangle{
    
    public :
    MetaScreen(){};
    ~MetaScreen(){
//        delete anchor;
        
    };
    MetaScreen(ofRectangle r,ofVec2f center,const string & _name = "defaultName"){
        
        set(r);
//        anchor = new ofxUIImageButton(center.x,center.y,ANCHOR_WIDTH,ANCHOR_HEIGHT,false,ofToDataPath("UI/screenAnchor.png"),_name);
//        anchor-> = false;
        
    }
    
    
    ofRectangle rectScreen(){
        return *((ofRectangle*)this);
    }
//    ofxUIImageButton *  anchor;

};




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
