/*
 *  ofxTweener.h
 *  openFrameworks
 *
 *  Created by Sander ter Braak on 26-08-10.
 *
 */

#include "ofMain.h"
#include "ofxTransitions.h"

#ifndef _OFXTWEEN
#define _OFXTWEEN

#define TWEENMODE_OVERRIDE 0x01
#define TWEENMODE_SEQUENCE 0x02

class Tween {
public:
	typedef float(ofxTransitions::* easeFunction)(float,float,float,float);
	float* _var;
	float _from, _to, _duration,_by, _useBezier;
	easeFunction _easeFunction;
	Poco::Timestamp _timestamp;
    bool contCall;
};


class ofxTweener : public ofBaseApp {

public:
	
	ofxTweener();
	
	void addTween(float &var, float to, float time, ofEvent<float> * ev=NULL,bool cC = false);
	void addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), ofEvent<float> * ev=NULL,bool cC = false);
	void addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, ofEvent<float> * ev=NULL,bool cC = false);
	void addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, float bezierPoint, ofEvent<float> * ev=NULL,bool cC=false);
    
	
	void removeTween(float &var);	
	void setTimeScale(float scale);
	void update(ofEventArgs &data);
	void removeAllTweens();	
	void setMode(int mode);
	
	int getTweenCount();	
	
	
private:
    
	float				_scale;
	ofxTransitions		a;
	bool				_override;
    
	void				addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, float bezierPoint, bool useBezier, ofEvent<float> * ev=NULL,bool cC = false);
	float				bezier(float b, float e, float t, float p);
	vector<Tween>		tweens;
    std::map<float *,ofEvent<float> *>   callbacks;
    
};


extern ofxTweener Tweener;
#endif