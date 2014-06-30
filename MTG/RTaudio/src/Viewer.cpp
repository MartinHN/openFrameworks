//
//  Viewer.cpp
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#include "Viewer.h"
#include "Misc.h"


void Viewer::setup(AnalyzerH * a){
    aH = a;
    cam.enableMouseInput();
    isCacheDirty = isGuiDirty =isViewDirty = true;


    setupGui();
    
    colors.push_back(ofColor::cadetBlue);
    colors.push_back(ofColor::red);
    colors.push_back(ofColor::rosyBrown);
    colors.push_back(ofColor::tomato);
    colors.push_back(ofColor::yellow);
    colors.push_back(ofColor::yellowGreen);
    colors.push_back(ofColor::chartreuse);
    colors.push_back(ofColor::darkCyan);
    
    
    viewR = ofGetHeight();

//	void setAspectRatio(float aspectRatio);
    
    resetCam = true;

    
    
    
}

void Viewer::setupGui(){
    
    
    registerParams();
    
    int ch = 0;
    int pad=10;
    guiconf = new ofxUISuperCanvas("Config",0,0,200,100);
    ch+=guiconf->getRect()->height+pad;
    guicam = new ofxPanel(settings);
    guicam->setPosition(ofPoint(0,ch));
    ch+= guicam->getHeight()+pad;
    guia = new ofxPanel(aH->analyzers[0]->settings);
    guia->setPosition(ofPoint(0,ch));
    ch += guia->getHeight() + pad;
    guis = new ofxPanel(aH->sH->slicers[0]->settings);
    guis->setPosition(ofPoint(0,ch));
    

    
    
    if(aH->getAnalyzerNames().size()>0){
        guiconf->addDropDownList("AnalyzerNames", aH->getAnalyzerNames());
    }
    if(aH->sH->getSlicerNames().size()>0){
        guiconf->addDropDownList("SlicerNames",aH->sH->getSlicerNames());
    }
    guiconf->addToggle("Recomp", false);
    
   	ofAddListener(guiconf->newGUIEvent,this,&Viewer::guiEvent);

}

void Viewer::registerParams(){
    MYPARAM(drawSlice,true,false,true);
    MYPARAM(autoZoom,false,false,true);
    MYPARAM(viewMouse,true,false,true);
    viewMouse.addListener(this,&Viewer::setViewMouse);
    MYPARAM(resetCam,true,false,true);
    autoZoom.addListener(this, &Viewer::autoScale);
    resetCam.addListener(this, &Viewer::resetView);
    MYPARAM(scale,ofVec3f(1),ofVec3f(0),ofVec3f(2));
    MYPARAM(center,ofVec3f(0),ofVec3f(-1),ofVec3f(1));

    
    settings.setName("Viewer");
    
    
}

void Viewer::setViewMouse(bool & b){
    if(b){
        cam.enableMouseInput();
    }
    else{
        cam.disableMouseInput();
    }
}

void Viewer::resetView(bool & b){
    if(b){
    cam.orbit(0, 0, viewR);
//        cam.enableOrtho();

//    cam.setDistance(viewBox.z);
        cam.setFov(10);//2*ofRadToDeg(atan(ofGetWidth()*2.0/(viewR))));//50
//    cam.setAspectRatio(viewBox.x*1.0/(viewBox.y));
        b= false;}
    
    
}


void Viewer::updateAnalyzerGui(){

    vector<string> curselected = ((ofxUIDropDownList *)guiconf->getWidget("AnalyzerNames"))->getSelectedNames();
    if(curselected.size()>0){
        aH->curAnalyzer = aH->get(curselected[0]);
        ofPoint pos = guia->getPosition();
        delete guia;
        guia = new ofxPanel(aH->curAnalyzer->settings);
        guia->setPosition(pos);
    }
    else{
        ofLogWarning("no analyzer Selected");
    }

}

void Viewer::updateSlicerGui(){
    
    vector<string> curselected = ((ofxUIDropDownList *)guiconf->getWidget("SlicerNames"))->getSelectedNames();
    if(curselected.size()>0){
        aH->sH->curSlicer = aH->sH->get(curselected[0]);
        ofPoint pos = guis->getPosition();
        delete guis;
        guis = new ofxPanel(aH->sH->curSlicer->settings);
        guis->setPosition(pos);
        
    }
    else{
        ofLogWarning("no slicer Selected");
    }
    
}


void Viewer::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();
	int kind = e.getKind();
	cout << "got event from: " << name << endl;
    if(name == "AnalyzerNames"){
        updateAnalyzerGui();
    }
    else if(name == "SlicerNames"){
        updateSlicerGui();
    }
    else if(name == "Recomp"){
        aH->sH->sliceIt();
        aH->analyzeIt();
    }
    

    
}

void Viewer::updateView(){
    if(drawSlice){
        
    }
    else{
        
    }


}

void Viewer::updateCache(){
    
//  
//    for (int i = 0 ; i < slices->size() ; i++){
//  
//    }
}


void Viewer::autoScale(bool & b){
    if(b){
    ofVec3f min(9999,99999,99999);
    ofVec3f max = -min;

    if(aH->curSlice
       !=NULL){
        
        for (int i = 0 ; i < aH->curSlice->size() ; i++){
            
            min.x = std::min(aH->curSlice->at(i).curpos.x,min.x);
            min.y = std::min(aH->curSlice->at(i).curpos.y,min.y);
            min.z = std::min(aH->curSlice->at(i).curpos.z,min.z);
            
            max.x = std::max(aH->curSlice->at(i).curpos.x,max.x);
            max.y = std::max(aH->curSlice->at(i).curpos.y,max.y);
            max.z = std::max(aH->curSlice->at(i).curpos.z,max.z);

        }

        scale = 1./(max-min);
        ofVec3f cpy = scale;
        if(max.x ==min.x){cpy.x=1;}
        if(max.y ==min.y){cpy.y=1;}
        if(max.z ==min.z){cpy.z=1;}
        scale=cpy;
    }
        center = -scale.get()*(min+max)/2;
        b = false;
    }
    
}
void Viewer::update(){


    if(isViewDirty){
        updateView();
        isViewDirty = false;
    }
    if(isCacheDirty){
        updateCache();
        isCacheDirty = false;
    }
    updateHoverSlice();
    
}

void Viewer::updateHoverSlice(){
    ofVec2f mouse(ofGetMouseX(),ofGetMouseY());
    float nearestDistance = 999999;
    hoverIdx=-1;
    for (int i = 0 ; i < aH->curSlice->size() ; i++){
        //TODO:Cache it
        ofVec3f pp( (center.get() + aH->curSlice->at(i).curpos* scale )*viewR );
        ofVec2f cur = cam.worldToScreen(pp);
        float distance = cur.distance(mouse);
		if(distance<20 && distance < nearestDistance) {
			nearestDistance = distance;
			hoverIdx = i;
		}
	}
    
}

void Viewer::draw(){
    if(displayFeatures){
        vector<string> ft = aH->sH->pool->getAxesNames();
        ofPoint anch(ofGetWidth()/3,20);
        for (int i = 0 ; i < ft.size() ; i++){
            ofDrawBitmapString(ofToString(i)+" "+ft[i],anch );
            anch.y+=20;
        }
    }
    ofPushMatrix();
    
    
    guiconf->draw();
    
//    ofTranslate(60,0);
    guis->draw();
    guia->draw();
    
    
    guicam->draw();
    
    ofPopMatrix();
    ofNoFill();


    cam.begin();
    if(aH->curSlice!=NULL){
        ofEnableAlphaBlending();

        ofPushMatrix();
        
        ofSetColor(ofColor::red);
        ofSetCircleResolution(60);
        ofCircle(0,0,0, viewR/2);
        ofRotateX(90);
        ofCircle(0,0,0, viewR/2);

        ofPopMatrix();
        ofFill();
        for (int i = 0 ; i < aH->curSlice->size() ; i++){
       ofPushMatrix();
            ofColor curcolor = colors[aH->curSlice->at(i).localid%colors.size()];
       ofSetColor(curcolor,150);
       ofVec3f pp( (center.get() + aH->curSlice->at(i).curpos* scale )*viewR );
       
       ofFill();
            float cscale = 1;
            float curl = pp.length();
       if(curl>viewR/2){
           cscale = viewR/(curl);
           pp.limit(viewR/2);
//           pp.z = 0;
           ofNoFill();

       }
       ofTranslate(pp);
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofVec3f curvec;
            float curangle;
            ofQuaternion q = cam.getGlobalOrientation();
            q.getRotate(curangle, curvec);
            ofRotate(curangle,curvec.x,curvec.y,curvec.z);
            
       ofRect(0,0,10*cscale,10*cscale);
         
       ofPopMatrix();
   }
}
    
    
    
    
    cam.end();
}


