//
//  VisuHandler.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 14/08/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#define MAXSCREENUNION 5

#include "VisuHandler.h"

VisuHandler::VisuHandler(){
  
    
}


void VisuHandler::setup(AttrCtl *attrctl,BlobHandler* bHin, int inwin, int inhin, int zdepthin, int * scrwin, int * scrhin){
    
    allParams.clear();
    allParams.setName("Visu");
    settings.setName("VisuHandler");
    
    zdepth = zdepthin;
    inw=inwin;
    inh= inhin;
    scrw=scrwin;
    scrh=scrhin;
    beat=0;
    attr = attrctl;
    
//    sharedImg.push_back(ofImage("images/background.png"));
  
    
    sH.setup(scrw,scrh,zdepth);
    sH.loadScreensPos();
    
    bH = bHin;
    
    pipeFbo.allocate(*scrw, *scrh);

    


}


VisuClass * VisuHandler::get(const string & name){
    for (int i = 0; i< visuList.size() ; i++){
        if(visuList[i]->settings.getName()==name) return visuList[i];
    }
    
    return NULL;  
}



void VisuHandler::update(){
    

    
    for(int i = 0;i<visuList.size();i++){
        if(visuList[i]->isActive&&!visuList[i]->isHighFPS){
            ofRectangle curS = sH.rectOfScreen(visuList[i]->screenN);
        visuList[i]->update(curS.width,curS.height);
        }
    }

    
}


void VisuHandler::updateHighFPS(){
   
    
    for(int i = 0;i<visuList.size();i++){
        if(visuList[i]->isActive&&visuList[i]->isHighFPS){
            ofRectangle curS = sH.rectOfScreen(visuList[i]->screenN);
            visuList[i]->update(curS.width,curS.height);
        }
    }
    
    
}




ofImage * VisuHandler::getSharedImg(int i){
    if(i>sharedImg.size())return;
    return &sharedImg[i];
}




void VisuHandler::registerParams(){
    bH->registerParams();
        attr->registerParam();
    
    MYPARAM(pipeblur,255,0,255);
    
    for(int i = 0 ; i< visuList.size();i++){
        visuList[i]->registerParam();
        allParams.add(visuList[i]->settings);
    }



}


const void VisuHandler::printallp(ofParameterGroup p){
    for (int i = 0 ; i<p.size();i++){
        cout<<p[i].getName()+ofToString(i)<<endl;
        if(p.type()==typeid(ofParameterGroup).name()){
            printallp(p);
            
            
        }
    } 
}

const void VisuHandler::draw(){
    // draw visu
   
    pipeFbo.begin();
    ofSetColor(0,0,0,pipeblur);
    ofRect(0,0,*scrw,*scrh);
    pipeFbo.end();
    
    glBlendEquation(GL_FUNC_ADD_EXT);
    
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA);

    for (int i = 0 ; i<visuList.size();i++){
        ofPushMatrix();
        ofPushView();
        ofPushStyle();
        ofTranslate(0,0,zdepth/2);
        if(visuList[i]->isActive&&visuList[i]->screenN>=0){
            int validScreen = sH.getValidScreen(visuList[i]->screenN);
            if(validScreen>=0){
                ofEnableAlphaBlending();
                if(visuList[i]->recopy){
                    int curn = validScreen%10;
                    do{
                        ofRectangle curR = sH.screenL[curn]->rectMax;
                        if (visuList[i]->isMapping)sH.screenL[curn]->screenWarp.loadMat();
                        else ofTranslate(curR.x,curR.y);
                        
                        visuList[i]->draw(curR.width,curR.height);
                        
                        if (visuList[i]->isMapping)sH.screenL[curn]->screenWarp.unloadMat();
                        else ofTranslate(-curR.x,-curR.y);
                        validScreen/=10;
                        curn = validScreen%10;
                    }while (curn>0);
                    
                }
                else{
                    
                    ofRectangle curR = sH.rectOfScreen(validScreen);
                    if (visuList[i]->isMapping&&validScreen<sH.screenL.size())sH.screenL[validScreen]->screenWarp.loadMat();
                    else ofTranslate(curR.x,curR.y);
                    
                    visuList[i]->draw(curR.width,curR.height);
                    
                    if (visuList[i]->isMapping&&validScreen<sH.screenL.size())sH.screenL[validScreen]->screenWarp.unloadMat();
                    else ofTranslate(-curR.x,-curR.y);
                    
                }


        }

    }
        ofPopStyle();
        ofPopView();
        ofPopMatrix();
    }
    
    
    ofSetColor(255);

}

ofParameterGroup * VisuHandler::getParamPtr(){
    return &allParams;
}

//VisuClass * get(const string name){
//    for(int i = 0 ; i<visuList[i]
//    
//    
//}

void VisuHandler::addVisu(VisuClass * v){
    visuList.push_back(v);
}


void VisuHandler::updateScreenSize(){
    
     pipeFbo.allocate(*scrw, *scrh);
    for (int i = 0 ; i< sH.screenL.size();i++){
        sH.screenL[i]->calcRectMax();
    }
}






