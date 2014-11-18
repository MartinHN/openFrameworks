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


class TweenParam {
public:
	typedef float(ofxTransitions::* easeFunction)(float,float,float,float);
	ofAbstractParameter* _var;
    vector<float> _from, _to;
    float _duration;
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
    void updateParams();
	void removeAllTweens();
	void setMode(int mode);
	
	int getTweenCount();
    
    
    template<typename T>
    void addTween(ofParameter<T>* var, T to, float time, float (ofxTransitions::*ease) (float,float,float,float)=&ofxTransitions::easeInOutElastic, float delay=0);
    
    template<typename T>
    void removeTween(ofParameter<T> var);
	
    
    vector<TweenParam>tweensP;
	
private:
    
	float				_scale;
	ofxTransitions		a;
	bool				_override;
    
	void				addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, float bezierPoint, bool useBezier, ofEvent<float> * ev=NULL,bool cC = false);
	float				bezier(float b, float e, float t, float p);
	vector<Tween>		tweens;
    std::map<float *,ofEvent<float> *>   callbacks;
    
    vector<float> getVectorFromParam(ofAbstractParameter* p);
    void setParam(ofAbstractParameter* parameter, vector<float> f);
};



template<typename T>
void ofxTweener::addTween(ofParameter<T>* var, T to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay){
    
    vector<float> from = getVectorFromParam(var);
    ofParameter<T> too = ofParameter<T>(to);
    
    float _delay = delay;
    Poco::Timestamp latest = 0;
    
    for(int i = 0; i < tweensP.size(); ++i){
		if( tweensP[i]._var->getGroupHierarchyNames() == var->getGroupHierarchyNames()) {
			// object already tweening, just kill the old one
			if(_override){
                tweensP[i]._from = getVectorFromParam(var);
                tweensP[i]._easeFunction = ease;
                tweensP[i]._to = getVectorFromParam(&too);
				tweensP[i]._easeFunction = ease;
				tweensP[i]._timestamp = Poco::Timestamp() + ((delay / _scale) * 1000000.0f) ;
				tweensP[i]._duration = (time / _scale) * 1000000.0f;
				return;
			}
			else {
				//sequence mode
				if((tweensP[i]._timestamp + tweensP[i]._duration) > latest){
					latest = (tweensP[i]._timestamp + tweens[i]._duration);
					delay = _delay + ((tweensP[i]._duration - tweensP[i]._timestamp.elapsed())/1000000.0f);
					from = getVectorFromParam(&too);
				}
			}
		}
	}
    
    TweenParam p;
    
    p._from = getVectorFromParam(var);
    p._var = var;
    p._easeFunction = ease;
    p._to = getVectorFromParam(&too);
    
    p._timestamp = Poco::Timestamp() + ((delay / _scale) * 1000000.0f) ;
	p._duration = (time / _scale) * 1000000.0f;
    
    tweensP.push_back(p);
    
}



template<typename T>
void ofxTweener::removeTween(ofParameter<T> var){
    for(vector<TweenParam>::iterator it = tweensP.begin(); it != tweensP.end(); ++it){
		if( it->_var->getGroupHierarchyNames() == var.getGroupHierarchyNames()) {
            tweensP.erase(it);
            break;
        }
    }
}




extern ofxTweener Tweener;
#endif