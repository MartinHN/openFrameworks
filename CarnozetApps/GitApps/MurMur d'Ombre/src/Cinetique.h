//
//  Cinetique.h
//  MursMurentDombres
//
//  Created by Aurélien Conil on 16/08/13.
//
//

#ifndef __MursMurentDombres__Cinetique__
#define __MursMurentDombres__Cinetique__

#include <iostream>
#include "ofMain.h"
//----------------------- LINE--------------------------------------------


class line
{
public:
    line(ofPoint pos, ofPoint angle, float t, float length);
    
    int update(ofPoint control, int channel, float argument);    // return 0 if the visual object is dead
    void draw();
    
private:
    
    ofPoint begin;
    ofPoint end;
    float angle;
    float length;
    float maxLength;
    float thickness;
    bool isBlack;
    
    int life, lifeTime;
    
};

//---------------------------------------------------------------------

class riverOfLine
{
public:
    riverOfLine();
    riverOfLine(ofPoint pos, ofPoint angle);
    ~riverOfLine();
    
    void init(ofPoint pos, ofPoint angle);
    int update( int channel, float argument);
    void draw();
    vector<line*> obj;
    
    
};



//--------------------------- Cinetique visu -------------------

class mechaniCircle {
    
    
public:
    
    mechaniCircle();
    mechaniCircle( ofPoint pos, float r1, float r2, float space, float speed);
    
    
    void update(ofPoint p, float angle);
    void draw(int h, int w);
    
    
    float angle1, angle2;
    float rad1, rad2, rad3, rad4;
    float space;
    
    ofPoint centroid;
    
    ofPath path1;
    
    float speed;
    
    

    
};

//--------------------------------- Animation Generale ------------------------

class FullscreenCinetique {

public:
    FullscreenCinetique();
    FullscreenCinetique( vector<ofPoint>* attrin, vector<int>* famillyin);
    ~FullscreenCinetique();
    void draw(int w, int h);
    
    
    float newAngle;
    
    //Display parameter
    bool isBeat;
    
    //object
    mechaniCircle mecha;
    
    //Line
    riverOfLine* line;
    bool isLine;
    
    
    //attractor
    ofPoint centroid;
    
    
    //Receive
    vector<ofPoint>* attr;
    vector<int>* familly;
    
};


#endif /* defined(__MursMurentDombres__Cinetique__) */
