//
//  DreamCatcher.h
//  MurMurDombre
//
//  Created by Delphine Grange on 12/12/2013.
//
//

#ifndef __MurMurDombre__DreamCatcher__
#define __MurMurDombre__DreamCatcher__

#include <iostream>
#include "ofMain.h"
#include <vector>
#include "ofxOsc.h"

#endif /* defined(__MurMurDombre__DreamCatcher__) */

//-------------------- Third level object ------------------

class particuleCircle
{
public:
    particuleCircle();
    particuleCircle(ofPoint pos, float speed, float maxSize);
    
    ofPoint pos;
    float radius;
    float maxSize;
    float speed;
    bool isDead;

    
    int update();    // return 0 if the visual object is dead
    void draw(int inw, int inh);
    
    
    
};
//-------------------- Second level object ------------------

class dreamObject  
{
public:
    dreamObject();
    dreamObject(ofImage* imgin, ofPoint pos, float size);
   
    ofPoint pos;
    float size;
    bool isActivated;
    bool isGrowing;
    int count;
    bool isDead;
    
    vector<particuleCircle> listOfCircle;
    
    int update(ofPoint p1, ofPoint p2);    // return 0 if the visual object is dead
    void draw(int inw, int inh, ofImage* imgin);


    
};



//-------------------- Main object --------------------------

class DreamCatcher
{
public:
    DreamCatcher();
    DreamCatcher( vector<ofPoint>* attrin,vector<int>* famillyin );
    
    
    void update();    // return 0 if the visual object is
    void draw(int inw, int inh);
    int density;
    float size;

    vector<ofPoint>* attr;
    vector<int>* familly;
    
    ofImage img;
    
    vector<dreamObject> listOfDream;
    
    ofxOscSender musicsender;
    bool setup;
    

};