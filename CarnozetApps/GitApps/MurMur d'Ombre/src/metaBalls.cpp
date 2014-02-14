//
//  metaBalls.cpp
//  MurMur d'Ombre
//
//  Created by martin hermant on 14/02/14.
//
//

#include "metaBalls.h"


metaBalls::metaBalls(VisuHandler * v):VisuClass(v){
    MYPARAM(rayon,0.f,0.f,1.f);
    MYPARAM(curFamilly,0,0,123);
    MYPARAM(color,ofVec4f(0),ofVec4f(0),ofVec4f(255));
    settings.setName("metaBalls");
    metaBall.loadImage("visu/point.png");

}


void metaBalls::update(int w, int h){
    
    points =  dad->attr->getFamilly(curFamilly);
    
}

void metaBalls::draw(int w, int h){
    
    if(points.size()>0){
    ofVec2f scale(w,h);
        ofSetColor(color.get().x, color.get().y, color.get().z);
        ofClear(0);
    for(int i = 0 ; i< points.size();i++){
        ofRectangle rect;
        rect.setFromCenter(points[i].x*w, points[i].y*h, rayon*w, rayon*h);
        metaBall.draw(rect);
    }

 
    }
}
