//
//  MultipleSine.h
//  MurMurDombre
//
//  Created by Delphine Grange on 15/12/2013.
//
//

#ifndef __MurMurDombre__MultipleSine__
#define __MurMurDombre__MultipleSine__

#include <iostream>
#include "ofMain.h"

#endif /* defined(__MurMurDombre__MultipleSine__) */

class Sine
{
public:
    Sine();
    Sine(float amp, float freq, float speed, float dec);
    
    float amp;
    float freq;
    float speed;
    ofPoint pos;
    float dec;
    float theta;
    
    void update(ofPoint att, bool newPos);
    void draw(int x, int maxw);
    
    
};

class MutlipleSine
{
public:
    
    MutlipleSine();
    MutlipleSine(vector<ofPoint>* attrin, vector<int>* famillyin );
    
    vector<ofPoint>* attr;
    vector<int>* familly;
    
    vector<Sine> listOfSine;
    
    void update();
    void draw(int inw, int inh);
    
    
};