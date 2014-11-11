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
    
    bool screensAnchorUp = false;
    
    int i = 0;
    sizex = TOTALRES_X*0.3568/2.0;
    curR =ofRectangle(0,0,sizex,sizey);
    screens[i] =new MetaScreen( curR, screensAnchorUp,"screenAnchor"+ofToString(i));
    cux+=sizex;
    
    i++;
    curR =ofRectangle(cux,0,sizex,sizey);
    screens[i] =new MetaScreen( curR, screensAnchorUp,"screenAnchor"+ofToString(i));
    
    cux+=sizex;
    curR =ofRectangle(cux,0,sizex,sizey);
    
    i++;
    sizex = TOTALRES_X*0.3120/2.0;
    screens[i] =new MetaScreen( curR, screensAnchorUp,"screenAnchor"+ofToString(i));
    cux+=sizex;
    curR =ofRectangle(cux,0,sizex,sizey);
    i++;
    screens[i] =new MetaScreen( curR, screensAnchorUp,"screenAnchor"+ofToString(i));
    cux+=sizex;
    curR =ofRectangle(cux,0,sizex,sizey);
    
    
    i++;
    sizex = TOTALRES_X*0.3310/2.0;
    screens[i] =new MetaScreen( curR, screensAnchorUp,"screenAnchor"+ofToString(i));
    cux+=sizex;
    curR =ofRectangle(cux,0,sizex,sizey);
    
    i++;
    screens[i] =new MetaScreen( curR, screensAnchorUp,"screenAnchor"+ofToString(i));
    cux+=sizex;
    
    resolution.set(cux,sizey);
    
    walls.resize(3);
    for(int ii = 0 ; ii < 3 ; ii++){
        
        walls[ii] = new MetaScreen(getWall(ii),!screensAnchorUp,"wscreenAnchor"+ofToString(ii));
    }
    
    
    ofRectangle screenR = ofRectangle(0,0,resolution.x,resolution.y);
    float anchXOffset;
    anchXOffset = walls[1]->getCenter().x - screenR.getCenter().x;
    full = new MetaScreen(screenR,screensAnchorUp,"fullScreen",ofVec2f(anchXOffset,0));
    
    
}



ofRectangle Screens::getWall(int i){
    return ofRectangle(screens[i*2]->x,screens[i*2]->y,screens[i*2]->width + screens[(i)*2+1]->width,screens[i*2]->height);
}
