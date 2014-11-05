//
//  Screens.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "Screens.h"



Screens * Screens::inst=NULL;


Screens::Screens(){
    

    
    screens.resize(6);
    
    int cux = 0;

    int sizex;
    int sizey= 1080;
    ofRectangle curR;
    
    
    int i = 0;
    sizex = TOTALRES_X*0.3568/2.0;
    curR =ofRectangle(0,0,sizex,sizey);
    screens[i] =new MetaScreen( curR, ofVec2f (curR.getCenter().x,curR.height-ANCHOR_HEIGHT/2.0),"screenAnchor"+ofToString(i));
    cux+=sizex;
    
    i++;
    curR =ofRectangle(cux,0,sizex,sizey);
    screens[i] =new MetaScreen( curR, ofVec2f (curR.getCenter().x,curR.height-ANCHOR_HEIGHT/2.0),"screenAnchor"+ofToString(i));

    cux+=sizex;
    curR =ofRectangle(cux,0,sizex,sizey);
    
    i++;
    sizex = TOTALRES_X*0.3120/2.0;
    screens[i] =new MetaScreen( curR, ofVec2f (curR.getCenter().x,curR.height-ANCHOR_HEIGHT/2.0),"screenAnchor"+ofToString(i));
    cux+=sizex;
    curR =ofRectangle(cux,0,sizex,sizey);
    i++;
    screens[i] =new MetaScreen( curR, ofVec2f (curR.getCenter().x,curR.height-ANCHOR_HEIGHT/2.0),"screenAnchor"+ofToString(i));
    cux+=sizex;
    curR =ofRectangle(cux,0,sizex,sizey);
    
    
    i++;
    sizex = TOTALRES_X*0.3310/2.0;
      screens[i] =new MetaScreen( curR, ofVec2f (curR.getCenter().x,curR.height-ANCHOR_HEIGHT/2.0),"screenAnchor"+ofToString(i));
    cux+=sizex;
    curR =ofRectangle(cux,0,sizex,sizey);
    
    i++;
    screens[i] =new MetaScreen( curR, ofVec2f (curR.getCenter().x,curR.height-ANCHOR_HEIGHT/2.0),"screenAnchor"+ofToString(i));
    cux+=sizex;
    
    resolution.set(cux,sizey);

    walls.resize(3);
    for(int ii = 0 ; ii < 3 ; ii++){

        walls[ii] = new MetaScreen(getWall(ii),ofVec2f (getWall(ii).getCenter().x,0+ANCHOR_HEIGHT/2.0),"wscreenAnchor"+ofToString(ii));
    }
    
    
    ofRectangle screenR = ofRectangle(0,0,resolution.x,resolution.y);
full = new MetaScreen(screenR,screenR.getCenter(),"fullScreen");
    
    
}



ofRectangle Screens::getWall(int i){
    
     return ofRectangle(screens[i*2]->x,screens[i*2]->y,screens[i*2]->width + screens[(i)*2+1]->width,screens[i*2]->height);
}
