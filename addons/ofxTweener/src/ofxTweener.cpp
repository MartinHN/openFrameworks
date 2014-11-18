/*
 *  ofxTweener.cpp
 *  openFrameworks
 *
 *  Created by Sander ter Braak on 26-08-10.
 *
 */

#include "ofxTweener.h"

ofxTweener Tweener;

ofxTweener::ofxTweener(){
	_scale = 1;
	setMode(TWEENMODE_OVERRIDE);
    ofAddListener(ofEvents().update,this, &ofxTweener::update);
}


void ofxTweener::addTween(float &var, float to, float time, ofEvent<float> * ev,bool cC){
	addTween(var,to,time, &ofxTransitions::easeOutExpo ,0,0,false, ev,cC);
}

void ofxTweener::addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), ofEvent<float> * ev,bool cC){
	addTween(var,to,time,ease,0,0,false, ev,cC);
}
void ofxTweener::addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, ofEvent<float> * ev,bool cC){
	addTween(var,to,time,ease,delay,0,false, ev,cC);
}
void ofxTweener::addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, float bezierPoint, ofEvent<float> * ev,bool cC){
	addTween(var,to,time,ease,delay, bezierPoint, true, ev,cC);
}




void ofxTweener::addTween(float &var, float to, float time, float (ofxTransitions::*ease) (float,float,float,float), float delay, float bezierPoint, bool useBezier,ofEvent<float> * ev,bool cC){
	float from = var;
	float _delay = delay;
	Poco::Timestamp latest = 0;
	
	for(int i = 0; i < tweens.size(); ++i){
		if(tweens[i]._var == &var) {
			// object already tweening, just kill the old one
			if(_override){
				tweens[i]._from = from;
				tweens[i]._to = to;
				tweens[i]._by = bezierPoint;
				tweens[i]._useBezier = useBezier;
				tweens[i]._easeFunction = ease;
				tweens[i]._timestamp = Poco::Timestamp() + ((delay / _scale) * 1000000.0f) ;
				tweens[i]._duration = (time / _scale) * 1000000.0f;
				return;
			}
			else {
				//sequence mode
				if((tweens[i]._timestamp + tweens[i]._duration) > latest){
					latest = (tweens[i]._timestamp + tweens[i]._duration);
					delay = _delay + ((tweens[i]._duration - tweens[i]._timestamp.elapsed())/1000000.0f);
					from = tweens[i]._to;
				}	
			}
		}
	}
	
	Tween t;
	t.contCall = cC;
	t._var = &var;
	t._from = from;
	t._to = to;
	t._by = bezierPoint;
	t._useBezier = useBezier;
	t._easeFunction = ease;
	t._timestamp = Poco::Timestamp() + ((delay / _scale) * 1000000.0f) ;
	t._duration = (time / _scale) * 1000000.0f;
	
	tweens.push_back(t);
	
    if (ev!=NULL) callbacks[t._var] = ev;
}

void ofxTweener::update(ofEventArgs &data){

	for(int i = tweens.size() -1; i >= 0; --i){
		if(float(tweens[i]._timestamp.elapsed()) >= float(tweens[i]._duration)){
			//tween is done

			bool found = false;
			if(!_override){
				//if not found anymore, place on exact place
				for(int j = 0; j < tweens.size(); ++j){
					if(tweens[j]._var == tweens[i]._var) {
						found = true;
						break;
					}
				}
			}
			if(!found) tweens[i]._var[0] = tweens[i]._to;
            
            map<float *,ofEvent<float> * >::iterator it = callbacks.find(tweens[i]._var);
            if(it != callbacks.end()) {
                ofNotifyEvent(*(it->second), *tweens[i]._var);
                callbacks.erase(it);
            }
            tweens.erase(tweens.begin() + i);
			
		}
		else if(float(tweens[i]._timestamp.elapsed()) > 0){
			//smaller than 0 would be delayed
			if(tweens[i]._useBezier) tweens[i]._var[0] = bezier(tweens[i]._from, tweens[i]._to ,(a.*tweens[i]._easeFunction )(float(tweens[i]._timestamp.elapsed()), 0, 1, float(tweens[i]._duration)), tweens[i]._by);
			else tweens[i]._var[0] = (a.*tweens[i]._easeFunction )(float(tweens[i]._timestamp.elapsed()), tweens[i]._from, tweens[i]._to - tweens[i]._from, float(tweens[i]._duration));
           
            // added continuous callBack : martinHN
            if(tweens[i].contCall){
                map<float *,ofEvent<float> * >::iterator it = callbacks.find(tweens[i]._var);
                if(it != callbacks.end()) {
                    ofNotifyEvent((*(it->second)), *tweens[i]._var,this);
                }
            }
		}
	}
    
    updateParams();
}


void ofxTweener::updateParams(){
    
    for(int i = tweensP.size() -1; i >= 0; --i){
		if(float(tweensP[i]._timestamp.elapsed()) >= float(tweensP[i]._duration)){
			//tween is done
            
			bool found = false;
			if(!_override){
				//if not found anymore, place on exact place
				for(int j = 0; j < tweens.size(); ++j){
					if(tweensP[j]._var->getGroupHierarchyNames() == tweensP[i]._var->getGroupHierarchyNames()) {
						found = true;
						break;
					}
				}
			}
			if(!found) setParam(tweensP[i]._var ,tweensP[i]._to);
            

            tweensP.erase(tweensP.begin() + i);
			
		}
		else if(float(tweensP[i]._timestamp.elapsed()) > 0){
			//smaller than 0 would be delayed
            ofAbstractParameter * parameter = tweensP[i]._var;
            if(parameter->type()==typeid(ofParameter<int>).name()){
                tweensP[i]._var->cast<int>() = (int)(a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[0], tweensP[i]._to[0] - tweensP[i]._from[0], float(tweensP[i]._duration));
              
            }else if(parameter->type()==typeid(ofParameter<float>).name()){
                tweensP[i]._var->cast<float>() = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[0], tweensP[i]._to[0] - tweensP[i]._from[0], float(tweensP[i]._duration));

                
            }else if(parameter->type()==typeid(ofParameter<ofVec2f>).name()){
                ofVec2f tmp;
                tmp.x = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[0], tweensP[i]._to[0] - tweensP[i]._from[0], float(tweensP[i]._duration));
                tmp.y = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[1], tweensP[i]._to[1] - tweensP[i]._from[1], float(tweensP[i]._duration));
                
                tweensP[i]._var->cast<ofVec2f>() = tmp;
            
            }else if(parameter->type()==typeid(ofParameter<ofVec3f>).name()){
                ofVec3f tmp;
                tmp.x = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[0], tweensP[i]._to[0] - tweensP[i]._from[0], float(tweensP[i]._duration));
                tmp.y = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[1], tweensP[i]._to[1] - tweensP[i]._from[1], float(tweensP[i]._duration));
                tmp.z = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[2], tweensP[i]._to[2] - tweensP[i]._from[2], float(tweensP[i]._duration));
                tweensP[i]._var->cast<ofVec3f>() = tmp;
            }
            else if(parameter->type()==typeid(ofParameter<ofRectangle>).name()){
                ofRectangle tmp;
          
                tmp.x = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[0], tweensP[i]._to[0] - tweensP[i]._from[0], float(tweensP[i]._duration));
                tmp.y = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[1], tweensP[i]._to[1] - tweensP[i]._from[1], float(tweensP[i]._duration));
                tmp.width = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[2], tweensP[i]._to[2] - tweensP[i]._from[2], float(tweensP[i]._duration));
                tmp.height = (a.*tweensP[i]._easeFunction )(float(tweensP[i]._timestamp.elapsed()), tweensP[i]._from[3], tweensP[i]._to[3] - tweensP[i]._from[3], float(tweensP[i]._duration));
                
                
                tweensP[i]._var->cast<ofRectangle>() = tmp;
                
                
            }
			 
            

		}
	}
    


        
    
}

void ofxTweener::setParam(ofAbstractParameter* parameter, vector<float> f){
    
    if(parameter->type()==typeid(ofParameter<int>).name()){
        parameter->cast<int>() = (int)f[0];
        
    }else if(parameter->type()==typeid(ofParameter<float>).name()){
       parameter->cast<float>() = f[0];
        
    }else if(parameter->type()==typeid(ofParameter<ofVec2f>).name()){
        ofVec2f tmpV(f[0],f[1]);

        parameter->cast<ofVec2f>()= tmpV;
        
    }else if(parameter->type()==typeid(ofParameter<ofVec3f>).name()){
        ofVec3f tmpV(f[0],f[1],f[2]);
        parameter->cast<ofVec3f>() = tmpV;
    }
    else if(parameter->type()==typeid(ofParameter<ofRectangle>).name()){
        ofRectangle tmpR(f[0],f[1],f[2],f[3]) ;
        parameter->cast<ofRectangle>() = tmpR;
       
    }

}


vector<float> ofxTweener::getVectorFromParam(ofAbstractParameter* parameter){
    vector<float> res;
    if(parameter->type()==typeid(ofParameter<int>).name()){
        res.push_back(parameter->cast<int>());
        
    }else if(parameter->type()==typeid(ofParameter<float>).name()){
        res.push_back(parameter->cast<float>());
        
    }else if(parameter->type()==typeid(ofParameter<ofVec2f>).name()){
        res.push_back(parameter->cast<ofVec2f>()->x);
        res.push_back(parameter->cast<ofVec2f>()->y);
        
    }else if(parameter->type()==typeid(ofParameter<ofVec3f>).name()){
        res.push_back(parameter->cast<ofVec3f>()->x);
        res.push_back(parameter->cast<ofVec3f>()->y);
        res.push_back(parameter->cast<ofVec3f>()->z);
    }
    else if(parameter->type()==typeid(ofParameter<ofRectangle>).name()){
        res.push_back(parameter->cast<ofRectangle>()->x);
        res.push_back(parameter->cast<ofRectangle>()->y);
        res.push_back(parameter->cast<ofRectangle>()->width);
        res.push_back(parameter->cast<ofRectangle>()->height);
        
    }
    
    return res;
}


void ofxTweener::removeTween(float &var){
	for(int i = 0; i < tweens.size(); i++){
		if(tweens[i]._var == &var) {
			// tween found, erase it
			tweens.erase(tweens.begin() + i);
			return;
		}
	}
}




float ofxTweener::bezier(float b, float e, float t, float p){
	return b + t*(2*(1-t)*(p-b) + t*(e - b));
}

void ofxTweener::removeAllTweens(){
	tweens.clear();
    tweensP.clear();
}
void ofxTweener::setMode(int mode){
	_override = (mode == TWEENMODE_OVERRIDE);
}

int ofxTweener::getTweenCount(){
	return int(tweens.size());
}

void ofxTweener::setTimeScale(float scale){
	_scale = scale;
}
