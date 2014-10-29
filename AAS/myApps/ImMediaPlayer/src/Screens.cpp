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
    int sizex = 900;
    int sizey= 1080;
    screens[0].set(0,0,sizex,sizey);
    cux+=sizex;
    sizex = 900;
    screens[1].set(cux,0,sizex,sizey);
    cux+=sizex;
    sizex = 900;
    screens[2].set(cux,0,sizex,sizey);
    cux+=sizex;
    sizex = 900;
    screens[3].set(cux,0,sizex,sizey);
    cux+=sizex;
    sizex = 900;
    screens[4].set(cux,0,sizex,sizey);
    cux+=sizex;
    sizex = 900;
    screens[5].set(cux,0,sizex,sizey);
    
    
    resolution.set(cux,sizey);

    for(int i = 0 ; i < 3 ; i++){
        walls.push_back(ScreenSpace(getWall(i),true));
    }
    
    
    

    
    
}



ofRectangle Screens::getWall(int i){
    
     return ofRectangle(screens[i*2].x,screens[i*2].y,screens[i*2].width + screens[(i+1)*2].width,screens[i*2].height);
}
