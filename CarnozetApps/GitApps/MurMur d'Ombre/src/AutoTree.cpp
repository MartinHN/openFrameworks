 //
//  AutoTree.cpp
//  MursMurentDombres
//
//  Created by Aurélien Conil on 16/08/13.
//
//

#include "AutoTree.h"

AutoTree::AutoTree(){
    
}

AutoTree::AutoTree(int w, int h, bool autostart)
{
    
    list.clear();
    
    width = w;
    height = h;
    
    myfbo = ofFbo();
    
    count = 0;
    
    imageLeaf.loadImage("images/leaf.png");
    
    if(autostart) init();
    
    
    
    
}

void AutoTree::init(){
    
    
    //    ofPoint center = ofPoint(width/2.0f, height-10);
    //    Branche b = Branche( center,1.57f, 200, true);
    //    b.angle = 1.57f;
    //    b.lifeTime = 200;
    //    b.finalLength = 200;
    //    list.push_back(b);
   // myfbo = ofFbo();
    myfbo.allocate(width,height, GL_RGBA);
    
    myfbo.begin();
    ofClear(255, 255, 255, 0);
    myfbo.end();
    list.clear();
    
}

void AutoTree::clear(){
    
    list.clear();
    
    if(myfbo.isAllocated()){
      //  myfbo = ofFbo();
      // myfbo.~ofFbo();
    }
    
}

void AutoTree::startStop(bool start){
    
    
    if(start)
    {
        
        clear();
        init();
        
    }
    else
    {
        
        clear();
        
    }
    
    
    
}


//--------------------------------------------------------------
void AutoTree::draw()
{
    ofPushStyle();  
    if (ofGetMousePressed())
    {
        reset(ofPoint(ofGetMouseX(), ofGetMouseY()));
    }
    
    
    std::vector<Branche>::iterator it;
    vector<Branche> newBranche;
    ofPoint newBrancheP ;
    float newBrancheA;
    int newBrancheL;
    bool endOfBranche = false;
    

    
    
    
        // Setup of FBO
        ofSetColor(0, 0, 0,255);
        myfbo.begin();
        
        //Update branches
        for (it = list.begin() ; it != list.end();  )
        {
        
             it->update();

            
            if(it->lifeTime < 1 && !(it->isChildCreated)  )
            {
                
                if(it->finalLength > 10)
                {
                
                    newBranche.push_back(*it);
                    
                }
                
                
                list.erase(it);
                
                
            }
            else
            {
            
            it->draw();
            ++it;
                
            }
            
        }
        
            
            
        
        myfbo.end();
        
        if(newBranche.size()>0)
        {
            
            for (it = newBranche.begin() ; it != newBranche.end(); ++it  )
            {
                
                createBranche(it->end, it->angle, it->length, true);
                
                
            }
            
        }
        
    
   // myfbo.updateTexture();
   
  myfbo.draw(0, 0);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void AutoTree::createBranche(ofPoint p, float a, int l, bool isEnd){
    
    int nb;
    Branche b;
    bool isClockWise ;
    
    if(l > 45){
    //nb = (int) ofRandom(1.8) + 1;
    nb = (int)  2;
    }
    else
    {
    
        //if(ofRandomf()>0.45) nb = 2;
        //else nb = 0;
        
        nb = (int) ofRandom( 3.1);
    
    }
    
    
    
    for(int i=0; i<nb; i++)
    {
        isClockWise = false;
        float angle = ofRandom(0.0, 0.75);
        if((i+1)%2 == 0 ){
            angle = -angle;
            isClockWise = true;
        }
        angle = angle+a;
        
        b = Branche(p, angle, l, isClockWise, &imageLeaf);
        list.push_back(b);
        
    }
    
    
    
}

//--------------------------------------------------------------
void AutoTree::reset(ofPoint p){
    
    // myfbo.begin();
    // ofClear(255, 255, 255);
    //myfbo.end();
    
    list.clear();
    
    //ofPoint center = ofPoint(width/2.0f, height-10);
    //    Branche b = Branche( p,1.57f, 200, true);
    //    b.angle = 1.57f;
    //    b.lifeTime = 200;
    //    b.finalLength = 200;
    //    list.push_back(b);
    
    Branche b = Branche( p,1.47f, 200, false, &imageLeaf);
   // b.angle = 1.39f;
    b.lifeTime = 150;
    b.finalLength = 150;
    list.push_back(b);
    
    b = Branche( p,0.8, 150, true, &imageLeaf);
   // b.angle = 1.3f;
    b.lifeTime = 140;
    b.finalLength =140;
    list.push_back(b);
    
    b = Branche( p , 2.2f, 150, false, &imageLeaf);
   // b.angle = 1.8f;
    b.lifeTime = 145;
    b.finalLength = 145;
    list.push_back(b);
    
}


//--------------------------------------- BRANCHE --------------

Branche::Branche(){
    
    
}

Branche::Branche( ofPoint b, float a, int actualL, bool isClockWise, ofImage* img){
    
    begin =b;
    end =b;
    
    isGrowing = true;
    isChildCreated = false;
    lifeTime = (int) ofRandom(actualL/1.5f, actualL/1.08f);
    finalLength = (float)lifeTime;
    clockWise=isClockWise;
    
    
    angle = a;
    length = 0;
    
    leaf = img;
    
    //leaf.loadImage("images/leaf.png");
    
    
}



void Branche::update(){
    
    int maxLife = 200;
    
    float speed = 4.0f;
    
    if(isGrowing){
        lifeTime --;
        length ++;
        float curve = (200/finalLength)*0.0007;
        if (clockWise) curve *= -1.0f;
        angle +=  curve;
    }
    if (lifeTime<0 ){
        isGrowing = false;
        isChildCreated = true;
    }
    
    if(isGrowing)
    {
        
        end = ofPoint( begin.x + length*cos(-angle), begin.y + length*sin(-angle));
        shape.addVertex(end);
    }
    
    
}

void Branche::draw(){
    
    
    
    float radius = finalLength/25.0f;
    if(radius<1.0f) radius = 1.0f;
    ofSetColor(0,0,0,255);
    
    if(isGrowing){
        
        ofPushMatrix();
        ofCircle(end, radius);
        
        if(ofRandomf()>0.90 && finalLength < 56)
        {
            
            ofTranslate(end);
            ofRotateZ(RAD_TO_DEG*angle);
            leaf->draw(0, 0);
            
        }
        ofPopMatrix();
        
    }
    
    
}
