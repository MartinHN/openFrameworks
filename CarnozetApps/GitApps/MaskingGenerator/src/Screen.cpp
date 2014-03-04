//
//  Screen.cpp
//  MaskingGenerator
//
//  Created by Conil Aurélien on 27/01/2014.
//
//

#include "Screen.h"



Screen::Screen(){}
//-----------------------------------------
Screen::Screen(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4){
    
    isActive = false;
    
    origin.push_back(p1);
    origin.push_back(p2);
    origin.push_back(p3);
    origin.push_back(p4);
    
    listOfPoint = origin;
    
    activePoint = -1;
    
    calcCentroid();
    
}


//-----------------------------------------
void Screen::dragPoint(ofPoint p){
    
    
    // Drag a point
    if( isActive && activePoint>=0 && activePoint <100)
    {
        listOfPoint.at(activePoint) = p;
        calcCentroid();
        
    }
    
    //Drag the entire screen
    if( isActive && activePoint==100){
        
        for( int i=0; i<listOfPoint.size(); i++){
            
            listOfPoint.at(i)  += (  p - centroid);
        }
        calcCentroid();
        
    }
    
    
    
    
    
}

//-----------------------------------------
void Screen::draw(){
    
    ofFill();
    ofSetColor(0, 0,255);
    ofPath path;
    
    for(int i=0; i< listOfPoint.size(); i++){
        
        
        path.lineTo(listOfPoint.at(i));
        
        if(i==activePoint){
            ofCircle(listOfPoint.at(i), 20);
        }
        else{
            ofCircle(listOfPoint.at(i) ,10);
        }
        
        
    }
    path.close();
    

    
    if(isActive){
        path.setFillColor(ofColor::white);
    }
    else{
        path.setFillColor(ofColor::red);
    }
    path.setFilled(true);
    
    ofSetColor(255);
    path.draw();
    
    ofNoFill();
    ofSetColor(0);
    if(isActive)
    ofCircle(centroid, 10);
    ofFill();
    
    
}

//-----------------------------------------
int Screen::findClosest(ofPoint mouse){
    
    float thresh = 30.0f;
    activePoint = -1;
    for(int j=0; j<listOfPoint.size(); j++)
    {
        

        ofPoint p = listOfPoint.at(j);
        float dist = p.distance(mouse);
        
        if(dist < thresh)
        {
            // According to the centroid
            activePoint = j;
        }
        


        
    }
    
    //According to centroid
    if(activePoint==-1){
        
        float dist = centroid.distance(mouse);
        
        if(dist < thresh)
        {
            // According to the centroid
            activePoint = 100;
        }
        
        
    }
    
    return activePoint;
    
    
}

//----------------------------------------
void Screen::calcCentroid(){
    
    ofPoint p = ofPoint(0,0);
    
    for(int i=0; i<listOfPoint.size(); i++){
        
        p += listOfPoint.at(i);

    }
    p /= listOfPoint.size();
    
    centroid = p;
    
}



//-----------------------------------------
void Screen::reset(){
    
    listOfPoint.clear();
    listOfPoint = origin;
}