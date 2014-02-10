//
//  Cinetique.cpp
//  MursMurentDombres
//
//  Created by Aurélien Conil on 16/08/13.
//
//

#include "Cinetique.h"



//------------------------------- Line ---------------------------------------
riverOfLine::riverOfLine(ofPoint pos, ofPoint angle){
    
    init( pos,  angle);
    
    
}

riverOfLine::riverOfLine(){

}

riverOfLine::~riverOfLine(){
    std::vector<line*>::iterator it;
    
    if(obj.size()>0){
    
        for (it = obj.begin() ; it != obj.end(); ++it )
        {
            delete (*it);
        }
        
    }
    
}

void riverOfLine::init( ofPoint pos, ofPoint angle){
    
    int nbLigne = (int) ofRandom(8);
    nbLigne = 20;
    
    
    
    
    
    
    float a = 600.0;
    float b = 100.0;
    
    for(int i=0;i<nbLigne; i++ )
    {
        
        ofPoint linePos = ofPoint(pos.x + ofRandom(-a, a), pos.y + ofRandom(-b, b));
        //ofPoint linePos = ofPoint(ofRandom(-a, a), ofRandom(-a, a), -1000);
        float thickness = ofRandom(3.0, 10.0);
        //float length = (int) (thickness*10 + 100 +ofRandom(90));
        float length = (int) (thickness*10 + 800 +ofRandom(200));
        
        line* myLine = new line(linePos, angle, thickness, length);
        
        obj.push_back(myLine);
        
    }
    
    
}

int riverOfLine::update( int channel, float argument){
    
    std::vector<line*>::iterator it;
    int totalOfLife = 0;
    ofPoint control = ofPoint();
    for (it = obj.begin() ; it != obj.end(); ++it)
    {
        totalOfLife += (*it)->update( control, channel, argument);
    }
    
    
    if (totalOfLife == 0)
        return 0;
    else return 1;
    
    
}

void riverOfLine::draw(){
    
    std::vector<line*>::iterator it;
    for (it = obj.begin() ; it != obj.end(); ++it)
    {
        (*it)->draw();
    }
    
    
}


//------------------------------------------------------------------------------------

line::line(ofPoint pos, ofPoint a, float t, float l){
    
    
    begin = pos;
    end = pos;
    angle = a.z;
    thickness = t;
    length = 0.0f;
    maxLength = l;
    length =  0;
    
    life = 0;
    lifeTime =(int) 2* l;
    
    isBlack = false;
    
}

int line::update(ofPoint control, int channel, float argument){
    
    
    //Update length and position
    int speed = 10;
    
    
    // if the line is growing
    if ( life < lifeTime / 2)
    {
        length += speed;
        end = ofPoint ( begin.x + length*cos(angle), begin.y +length*sin(angle));
        
    }
    else
    {
        
        length -=speed;
        begin = ofPoint (end.x - length*cos(angle),end.y - length*sin(angle));
        
    }
    
    life +=speed;
    if(life>lifeTime) return 0;
    else return 1;
    
}

void line::draw(){
    
    ofSetLineWidth(thickness);
    if(isBlack)
        ofSetColor(0);
    else
        ofSetColor(255);
    ofLine(begin, end);
    
    
}



//------------------------------------- Cinetique visu-----------------------------

mechaniCircle::mechaniCircle(){}


mechaniCircle::mechaniCircle( ofPoint pos, float r1, float r2, float spa, float spe){
    
    
    centroid = pos;
    rad1 = r1;
    rad2 = r2;
    
    space = spa;
    speed = spe;
    
    path1.setColor(ofColor(255));
    path1.setFilled(TRUE);
    
    
}


void mechaniCircle::update( ofPoint newpos, float angle)
{
    
    
    angle1 = angle;
    
    ofPoint a,b,c,d;
    
    a = ofPoint( rad1*cos(angle1 + space), rad1*sin(angle1 + space));
    b = ofPoint( rad2*cos(angle1 + space), rad2*sin(angle1 + space));
    c = ofPoint( rad2*cos(angle1 + 90 - space), rad2*sin(angle1 + 90 -space));
    d = ofPoint( rad1*cos(angle1 + 90 - 2.0*space*cos(angle*0.1)), rad1*sin(angle1 + 90 -2.0*space*cos(angle*0.1)));
    
    path1.clear();
    
    path1.curveTo(a);
    path1.lineTo(b);
    path1.arc(centroid, rad1, rad1, angle1 + space, angle1 +90 - space, true);
    //path1.curveTo(c);
    path1.lineTo(d);
    path1.curveTo(a);
    
    centroid = newpos;
    
}

void mechaniCircle::draw(int w, int h)
{
    
    ofPushMatrix();
    ofTranslate(centroid);
    path1.draw();
    ofRotateZ(90);
    path1.draw();
    ofRotateZ(90);
    path1.draw();
    ofRotateZ(90);
    path1.draw();
    
    ofPopMatrix();
    
}

//-------------------------------------------------------------
// Animation générale
//--------------------------------------------------------------

FullscreenCinetique::FullscreenCinetique(){}

FullscreenCinetique::FullscreenCinetique( vector<ofPoint>* attrin, vector<int>* famillyin){
    
    
    float gravity = 0.3;
    float mass = 1.0;
    float spSize = 3.0;
    
    attr = attrin;
    familly = famillyin;
    
    mecha = mechaniCircle(ofPoint (0,0), 300, 100, 2, 1.2);
    
    isLine = false;
    
    line = NULL;
    
}

FullscreenCinetique::~FullscreenCinetique()
{
    
    if(line != NULL)
        delete (line);
    
}




//--------------------------------------------------------------
void FullscreenCinetique::draw(int w, int h){
    

    int nattr = attr->size();
    if(nattr>0)
    {
        centroid.x=attr->at(0).x*w;
        centroid.y=attr->at(0).y*h;

    }
    
    
    //Debug
    centroid = ofPoint(w/2.0f, h/2.0f);
    
    
    //AUto angle
    float autoangle = ofGetElapsedTimef()*1.2;
    
    //mecha.update(ofPoint(spr.x, spr.y));
    // mecha.update(centroid, DEG_TO_RAD*newAngle);
    
    mecha.update(centroid, autoangle);
    
    // DRAW cinetique animation

    mecha.draw(w,h);

    
    //Line
    if (isLine && line!= NULL)
    {
        
        int res = line->update(0, 0);
        line->draw();
        
        
        if(res==0)
        {  // anim is finished and must delete
            delete (line);
            isLine = false;
        }
        
        
    }
    else
    {  // no animation, possibility to create a new one
        
        if( ofRandomf()>0.9)
        {
            
            ofPoint posLine = ofPoint( w/4.0f,0);
            ofPoint angleLine = ofPoint(0,0,45);
            
            line = new riverOfLine(posLine, angleLine);
            isLine = true;
            
        }
        
    }
    
    
}


