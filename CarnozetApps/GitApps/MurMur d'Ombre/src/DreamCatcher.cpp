//
//  DreamCatcher.cpp
//  MurMurDombre
//
//  Created by Delphine Grange on 12/12/2013.
//
//

#include "DreamCatcher.h"

DreamCatcher::DreamCatcher(){}

DreamCatcher::DreamCatcher( vector<ofPoint>* attrin,vector<int>* famillyin ){
    
    
    attr = attrin;
    familly = famillyin;
    
    density = 12;
    size = 0.4f;
    
    img.loadImage("images/dreamcatcher.png");
    
    listOfDream.clear();
    
    for(int i=0; i< density; i++)
    {
        
        float finalSize = size + ofRandom(-0.2, +0.2);
        
        dreamObject obj = dreamObject( &img, ofPoint( ofRandom(0.1,0.9), ofRandom(0.1,0.7)), finalSize );
        
        listOfDream.push_back(obj);
        
        
    }
    int b;
    
    setup = false;
    
    
}


void DreamCatcher::update(){
    
    ofPoint control1;
    ofPoint control2;
    bool first = false;
    
    //OSC setup osc if necessary
    if(!setup) {
        
        musicsender.setup("localhost", 12350);
        setup = true;
    }
    
    if(attr->size()>0 && ( attr->size()==familly->size()))
    {
        
        
        for( int i=0; i<(attr->size());i++)
        {
            if( familly->at(i) == 1){
            
                if(!first) // 1st hand
                {
                    control1 = ofPoint(attr->at(i).x , attr->at(i).y );
                    first = true;
                }
                else // 2nd hand
                {
                    control2 = ofPoint(attr->at(i).x , attr->at(i).y );
                }
                
                
            }
        }
        

        
        
    }
    else
    {
        
        control1 = ofPoint( -1, -1);
        control2 = ofPoint ( -1,-1);
    }
        
        
    // Update point
    for(vector<dreamObject>::iterator i = listOfDream.begin(); i != listOfDream.end();)
    {
        // 0 = dead ; 1 = normal ; 2 = send a note
        int res = i->update(control1, control2);
        
        if( res==0) // dreamCatcher is dead : send a note via osc
        {
            
            listOfDream.erase(i);
        }
        else{
            
            if(res==2){
            
                ofxOscMessage msg = ofxOscMessage();
                
                msg.setAddress("xylo");
                msg.addFloatArg( 64 + ((i->size - size) * 64));
                
                musicsender.sendMessage(msg);
                
            }
            
            i++;
        }
        
        
        
    }
    
    // Create new dreamCatcher
    int numToCreate = density - listOfDream.size();
    
    for(int i=0; i< numToCreate; i++)
    {
        
        float finalSize = size + ofRandom(-0.2, +0.2);
        
        dreamObject obj = dreamObject( &img, ofPoint( ofRandom(0.1,0.9), ofRandom(0.1,0.7)), finalSize );
        
        listOfDream.push_back(obj);
        
        
    }
        
        
        
        
    
    
}

void DreamCatcher::draw( int inw,int inh){
    
   
    int a;
    
    for(vector<dreamObject>::iterator i = listOfDream.begin(); i != listOfDream.end();++i)
    {
    
        i->draw(inw, inh, &img);
        
    }
    
    if(attr->size()>0 && ( attr->size()==familly->size()))
    {
    
    for( int j=0; j<(attr->size());j++)
    {
        if( familly->at(j) == 1)
        {
         
            ofSetColor(58,45,76);
            ofPoint p = attr->at(j);
            ofCircle( p.x * inw, p.y * inh, 20);

            
            
        }
    }
        
    }
    
    

    
    
}


//-----------------------------------------------------------------------------

dreamObject::dreamObject(){}

dreamObject::dreamObject(ofImage* imgin, ofPoint posin, float sizein){

    
    pos = posin;
    size = sizein;
    
    isActivated = false;
    isGrowing = true;
    isDead = false;
    count = 0;
    
    listOfCircle.clear();
    
    
}

int dreamObject::update(ofPoint control1, ofPoint control2){
    
    
    
    //Activation update
    float radius = 0.05;
    float dist1 =  pos.distance(control1);
    float dist2 =  pos.distance(control2);
    
        
    if(  ( dist1 < radius || dist2 < radius ) && !isActivated && !isGrowing )
    {
        
        isActivated = true;
        
        //create 15 circle
        for(int i=0; i< 15; i++)
        {
            ofPoint newpos = ofPoint( pos.x + ofRandom(-0.01, 0.01) , pos.y + ofRandom(-0.01, 0.01));
            particuleCircle obj = particuleCircle(newpos, ofRandom(0.9, 2.1), ofRandom(size*30,size*60));
            listOfCircle.push_back(obj);
        }
        
    
    }
    
    
    // Growing update
    if( isGrowing)
    {
        
        count++;
        
        if(count > 100)
        {
            count = 0;
            isGrowing = false;
        }
        
    }
    
    // Animation of activation update
    if ( isActivated)
    {
        
        for(vector<particuleCircle>::iterator i = listOfCircle.begin(); i != listOfCircle.end();)
        {
            
            int res = i->update();
            
            
            if( res==0) // circle is dead
                listOfCircle.erase(i);
            else
                i++;
            
        }
        
        count++;
        
        // Finally die if all the circle are dead
        if(listOfCircle.size()<1) isDead = true;
        
        
        
    }
    
    
    
    
    if(isDead) return 0;
    else if(isActivated && count == 1) return 2;
    else return 1;
}


void dreamObject::draw(int inw, int inh, ofImage* imgin){
    
    float w = imgin->width*size;
    float h = imgin->height*size;
    
    float intensity;
    
    if(isGrowing) intensity = (count*1.0f/100)*255 ;
    else if (isActivated) intensity = listOfCircle.size()*17;
    else intensity =  255;
    
    ofPushMatrix();
    
    ofTranslate( pos.x *inw, pos.y*inh );
    ofRotateZ(ofGetElapsedTimef()*15.1f);
    
    ofSetColor(intensity);
    imgin->draw(-(w/2.0f),-(h/2.0f), w, h);
    
    ofPopMatrix();
    
    if( isActivated){
        ofPushMatrix();
        for(vector<particuleCircle>::iterator i = listOfCircle.begin(); i != listOfCircle.end();++i)
        {
            
            i->draw(inw, inh);

        }
        ofPopMatrix();
    }
    
}

//-------------------------------------------------------------------

particuleCircle::particuleCircle(){}

particuleCircle::particuleCircle(ofPoint posin, float speedin, float maxSizein){
    
    pos = posin;
    speed = speedin;
    maxSize = maxSizein;
    
    radius = 0;
    isDead = false;
    
    
}


int particuleCircle::update(){
    
    radius += speed;
    int res;
    
    if(radius > maxSize){
        isDead = true;
        res = 0;    }
    else{
        res = 1;    }
    
    return res;
    
}

void particuleCircle::draw(int inw, int inh ){
   
    ofPushMatrix();
    ofSetColor(255, 255, 255, 125);
    ofNoFill();
    ofSetLineWidth(2.0f);
    ofCircle(pos.x * inw, pos.y * inh, radius);
    ofPopMatrix();
    
}
                        
