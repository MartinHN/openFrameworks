//
//  Screen.h
//  MaskingGenerator
//
//  Created by Conil Aurélien on 27/01/2014.
//
//

#ifndef __MaskingGenerator__Screen__
#define __MaskingGenerator__Screen__

#include <iostream>
#include "ofMain.h"

#endif /* defined(__MaskingGenerator__Screen__) */


class Screen {
    
    
public:
    
    Screen();
    Screen(ofPoint p1, ofPoint p2, ofPoint p3, ofPoint p4);
    vector<ofPoint> listOfPoint;
    vector<ofPoint> origin;
    ofPoint centroid;
    bool isActive;
    int activePoint;
    
    void draw();
    void dragPoint(ofPoint p);
    void reset();
    int findClosest(ofPoint p);
    void calcCentroid();
    
    
    
};