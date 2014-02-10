//
//  MultipleSine.cpp
//  MurMurDombre
//
//  Created by Delphine Grange on 15/12/2013.
//
//

#include "MultipleSine.h"


Sine::Sine(){}

Sine::Sine(float ampin, float freqin, float speedin, float decin){
    
    amp = ampin;
    freq = freqin;
    speed = speedin;
    dec = decin;
    theta = ofRandom(3.14);
    
}

void Sine::update(ofPoint att, bool newPos)
{
    
    
    
    if(newPos){
    pos = att;
        theta = 0;
        
    }else{
    theta = theta+speed+0.2;
    }
    
}

void Sine::draw(int x, int maxw){
    
    float X = x-(pos.x*maxw);
    float amplitude = amp * cos(0.1*theta);
    
    float y = (amplitude )*(sin( 2*3.14*freq*(X*1.0f) + theta) );
    ofSetColor(255, 255, 255, 255);
    
    ofCircle(x*1.0f, y, 1.0f);
    
}

//--------------------------------------------------------------------

MutlipleSine::MutlipleSine(){}


MutlipleSine::MutlipleSine(vector<ofPoint>* attrin, vector<int>* famillyin ){
    
    attr = attrin;
    familly = famillyin;
    
    //Sine(float amp, float freq, float speed, float dec)
    
    Sine s1 = Sine(50, 20, 0.050f, 0.05f);
    listOfSine.push_back(s1);
    
    s1 = Sine(54,20.5, 0.060f, 0.05f);
    listOfSine.push_back(s1);
    
    s1 = Sine(46,20, 0.043f, 0.05f);
    listOfSine.push_back(s1);
    
    s1 = Sine(52,21, 0.030f, 0.05f);
    listOfSine.push_back(s1);
    
    
    //s1 = Sine(49, 10, 0.22f, 1.0f);
    //listOfSine.push_back(s1);
    
    
}

void MutlipleSine::update(){
  
    ofPoint att = ofPoint(0,0);
    
    if(attr->size()>0 )
    {
        att = ofPoint(attr->at(0).x , attr->at(0).y);
    }
    
    
    for(vector<Sine>::iterator i = listOfSine.begin(); i != listOfSine.end();i++)
    {
        if(att.x * att.y > 0)
            i->update(att, true);
        else
            i->update(att, false);
    }
    
}

void MutlipleSine::draw(int inw, int inh){
    
    
    
    ofTranslate(0, inh/2.0);
    
    for(int x=0; x<inw; x++)
    {
        ofPushMatrix();
        for(vector<Sine>::iterator i = listOfSine.begin(); i != listOfSine.end();i++)
        {
            i->draw(x, inw);
            ofTranslate(0,0, -1.0f);
        }
        ofPopMatrix();
        
    }
    
    
    
}


