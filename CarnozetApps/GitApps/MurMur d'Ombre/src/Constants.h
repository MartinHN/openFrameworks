//
//  Constants.h
//  allAddonsExample
//
//  Created by Tinmar on 14/11/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#define LIVEBLUR

#define VISU_OSC_IN 12344
#define VISU_OSC_OUT 12346

#define VISU_OSC_IP_OUT "localhost"

//#define GUIMODE


// Struct for doing PingPong quickly and easy
//
// Because on GPU you can´t write over the texture that you are reading we are
// using to pair of ofFbo attached together on what we call pingPongBuffer 
// Learn more about Ping-Pong at:
//
// http://www.comp.nus.edu/~ashwinna/docs/PingPong_FBO.pdf
// http://www.seas.upenn.edu/~cis565/fbo.htm#setupgl4
//
struct pingPongBuffer {
public:
    void allocate( int _width, int _height, int _internalformat = GL_RGBA, float _dissipation = 1.0f){
        // Allocate
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(_width,_height, _internalformat );
        }
        
        // Clean
        clear();
        
        // Set everything to 0
        flag = 0;
        swap();
        flag = 0;
    }
    
    void swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }
    
    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s  
    int     flag;       // Integer for making a quick swap
};


struct forces{
    
    float mousex;
    float mousey;
    float mousez;
    bool isClicked;
    float screenw;
    float screenh;
    int lin;
    int col;
    float timestep;
    
    float Gx;
    float Gy;
    float Gz;
    
    float random;
    

    
    //Friction
    bool friction;
    float fr;
    float frmin;
    float minv;
    float maxv;
    
    
    //Origin
    bool origin;
    bool circle;
    bool forceorigin;
    float korigin;
    float zorigin;
    
    //Border
    bool border;
    float kborder;
    float zborder;
    float rborder;
    float zminborder;
    float zmaxborder;
    
    //Net
    bool neth;
    float kneth;
    float zneth;
    float lneth;
    bool netshot;
    
    bool netw;
    float knetw;
    float znetw;
    float lnetw;
    
    //MouseSpring
    int mousespring;
    float kmspring;
    float knmspring;
    int nkmspring;
    float zmspring;
    float rmspring;
    
    
    //MouseRotation
    bool mouserotation;
    
    float rmrotation;
    float smrotation;
    
    //Mousepick
    bool mousepick;
    float rmpick;
    float kmpick;
    float zmpick;
    
    int mousepicker;
    float rmpicker;
    float kmpicker;
    float zmpicker;
    
    int mousedamp;
    float rmdamp;
    float dmdamp;
    
    //CONTOURSPRING
    bool contourspring;
    float kctrspring;
    float zctrspring;
    float rctrspring;
    
    //MOUSEGRAVITY
    bool mousegravity;
    float mmgravity;
    float rinmgravity;
    float minmgravity;
    
    
    // XSPRING   
    bool xspring;
    float kxspring;
    float zxspring;
    float xori;
    
    
    //YSPRING
    bool yspring;
    float kyspring;
    float zyspring;
    float yori;
    
    
    
    
    //ZSPRING
    bool zspring;
    float kzspring;
    float zzspring;
    float zori;
    
    
    
    
    
    //
    bool lorenz;
    float lorenzatt;
    
    // PLAN FORCE
    ofVec3f plan;
    
    
    int planforce;
    float dplanforce;
    float kplanforce;
    float zplanforce;
    
    
    
    
    // ATTRACTORS
    
        bool split;
   
    bool isAtt;
    
    
    int attrspring0;
    int attrspring1;
    int attrspring2;
    
    
    float raspring0;
    float raspring1;
    float raspring2;
    
    float kaspring0;
    float kaspring1;
    float kaspring2;
    
    float zaspring0;
    float zaspring1;
    float zaspring2;
    
    
    Boolean attrgravity0;
    Boolean attrgravity1;
    Boolean attrgravity2;
    
    float rinagravity0;
    float rinagravity1;
    float rinagravity2;
    
    float minagravity0;
    float minagravity1;
    float minagravity2;
    
    float magravity0;
    float magravity1;
    float magravity2;
    
    
    Boolean attrrotation0;
    Boolean attrrotation1;
    Boolean attrrotation2;
    
    float sarotation0;
    float sarotation1;
    float sarotation2;
    
    float rarotation0;
    float rarotation1;
    float rarotation2;
    
    int attrdamp0;
    int attrdamp1;
    int attrdamp2;
    
    float radamp0;
    float radamp1;
    float radamp2;
    
    float cadamp0;
    float cadamp1;
    float cadamp2;
    
    
    

    
    
    
    
    //gradient
    bool splitgradient;
    int gradtype;
    int gradsize;
    float mingrad;
    float maxgrad;
    float mingrad2;
    float maxgrad2;
    
    
    //blob
    Boolean blobin;
    float blobforce;
    float blobz;
    float bloboutvel;
   bool blobxinvert;
bool    blobyinvert;
bool    blobmirrory;
bool    blobmirrorx;
    float blobblur;
    
};


static int computeRatio = 2;

static int FPS = 23;
// screen size

static int scrw = 1280;
static int scrh = 800;


static float calcAngle(ofVec2f cur, ofVec2f next){
    
    float teta;
    
    
    //Normalize
    ofVec2f nCur = cur.normalize();
    ofVec2f nNext = next.normalize();
    ofVec2f orthonCur = ofVec2f(-nCur.y, nCur.x);
    
    float cosTeta = nCur.dot(nNext);
    float sinTeta = orthonCur.dot(nNext);
    
    if(sinTeta>0)teta = acos(cosTeta);
    else teta = -acos(cosTeta);
    
    
    
    return teta;
    
    
}



template <class C> void FreeClear( C & cntr ) {
    for ( typename C::iterator it = cntr.begin(); 
         it != cntr.end(); ++it ) {
    	delete * it;
    }
    cntr.clear();
}

