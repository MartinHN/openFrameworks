//
//  Viewer.cpp
//  RTaudio
//
//  Created by martin hermant on 19/03/14.
//
//

#include "Viewer.h"
#include "Misc.h"


Viewer::Viewer(){
    
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
    
    
    
    guip = new ofxPanel(settings);
    gui = new ofxUISuperCanvas("Axes",ofGetWidth()/2,0,300,200);
    
//    
//    if(analyzers->getAxes().size()>0){
//        
//        gui->addDropDownList("Axesx", p->getAxesNames());
//        gui->addDropDownList("Axesy", p->getAxesNames());
//        gui->addDropDownList("Axesz", p->getAxesNames());
//    }
}

void Viewer::registerParams(){
    MYPARAM(drawSlice,true,false,true);
    MYPARAM(autoZoom,false,false,true);
    MYPARAM(resetCam,true,false,true);
    autoZoom.addListener(this, &Viewer::autoScale);
    resetCam.addListener(this, &Viewer::resetView);
    MYPARAM(scale,ofVec3f(1),ofVec3f(0),ofVec3f(2));
    MYPARAM(center,ofVec3f(0),ofVec3f(-1),ofVec3f(1));
    
    
    settings.setName("Viewer");
    
    
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


void Viewer::updateGui(){
//    ((ofxUIDropDownList*)gui->getWidget("Axesx"))->clearToggles();
//    axesx = p->getAxesNames();
//    ((ofxUIDropDownList*)gui->getWidget("Axesx"))->addToggles(axesx);
//    ((ofxUIDropDownList*)gui->getWidget("Axesy"))->clearToggles();
//    axesy = p->getAxesNames();
//    ((ofxUIDropDownList*)gui->getWidget("Axesy"))->addToggles(axesy);
//    ((ofxUIDropDownList*)gui->getWidget("Axesz"))->clearToggles();
//    axesz = p->getAxesNames();
//    ((ofxUIDropDownList*)gui->getWidget("Axesz"))->addToggles(axesz);
//    
//    


}

void Viewer::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();
	int kind = e.getKind();
	cout << "got event from: " << name << endl;
    if(name == "Axesx"){
        
    }
    else if(name == "Axesy"){
        
    }
    else if(name == "Axesz"){
        
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

    if(aH->curslice!=NULL){
        
        for (int i = 0 ; i < aH->curslice->size() ; i++){
            
            min.x = std::min(aH->curslice->at(i).curpos.x,min.x);
            min.y = std::min(aH->curslice->at(i).curpos.x,min.y);
            min.z = std::min(aH->curslice->at(i).curpos.x,min.z);
            
            max.x = std::max(aH->curslice->at(i).curpos.x,max.x);
            max.y = std::max(aH->curslice->at(i).curpos.x,max.y);
            max.z = std::max(aH->curslice->at(i).curpos.x,max.z);

        }
        scale = 1./(max-min);
        
    }
        center = ofVec3f(0);
        b = false;
    }
    
}
void Viewer::update(){

    if(isGuiDirty){
        updateGui();
        isGuiDirty = false;
    }
    if(isViewDirty){
        updateView();
        isViewDirty = false;
    }
    if(isCacheDirty){
        updateCache();
        isCacheDirty = false;
    }

}

void Viewer::draw(){
    
    gui->draw();
    guip->draw();
    ofNoFill();


    cam.begin();
    if(aH->curslice!=NULL){
        ofEnableAlphaBlending();

        ofPushMatrix();
        
        ofSetColor(ofColor::red);
        ofSetCircleResolution(60);
        ofCircle(0,0,0, viewR/2);
        ofRotateX(90);
        ofCircle(0,0,0, viewR/2);

        ofPopMatrix();
        ofFill();
        for (int i = 0 ; i < aH->curslice->size() ; i++){
       ofPushMatrix();
       ofSetColor(colors[aH->curslice->at(i).localid%colors.size()],150);
       ofVec3f pp( (center.get() + aH->curslice->at(i).curpos* scale )*viewR );
       
       ofFill();
       if(pp.length()>viewR/2){
           pp.limit(viewR/2);
//           pp.z = 0;
           ofNoFill();
           cout<<"out"<<endl;
       }
       ofTranslate(pp);
       ofRect(0,0,10,10);
       ofPopMatrix();
   }
}
    
    
    
    
    cam.end();
}


