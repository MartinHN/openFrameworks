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


void VisuHandler::setup(AttrCtl *attrctl, int inwin, int inhin, int zdepthin, int * scrwin, int * scrhin){
    
    allParams.clear();
    allParams.setName("Visu");
    zdepth = zdepthin;
    inw=inwin;
    inh= inhin;
    scrw=scrwin;
    scrh=scrhin;
    beat=0;
    attr = attrctl;
    
    sharedImg.push_back(ofImage("images/background.png"));
  
    
    sH.setup(scrw,scrh,zdepth);
    sH.loadScreensPos();
    
    
    

}

void VisuHandler::setupSyphon(ofShader *blurXin,ofShader *blurYin){
#ifdef syphon
    blobClient.setup();
    blobClient.setApplicationName("Simple Server");
    blobClient.setServerName("");
   syphonTex.allocate(inw,inh,GL_RGB32F);

    blurX = blurXin;
    blurY = blurYin;
    
#endif
    
}
VisuClass * VisuHandler::get(const string & name){
    for (int i = 0; i< visuList.size() ; i++){
        if(visuList[i]->settings.getName()==name) return visuList[i];
    }
    
    return NULL;  
}

#ifdef syphon
void VisuHandler::blurblob(){
    ofPushMatrix();
    ofPushStyle();
    ofPushView();
    
    glBlendEquation(GL_FUNC_ADD_EXT);
    
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA);
    
    syphonTex.dst->begin();
    ofSetColor(255);
    blurX->begin();
    blurX->setUniform1f("blurAmnt", blobBlur);
    blobClient.draw(0,0,inw,inh);
    blurX->end();
    syphonTex.dst->end();
     glFlush();

    syphonTex.swap();

    syphonTex.dst->begin();
    blurY->begin();
    blurY->setUniform1f("blurAmnt", blobBlur);
    syphonTex.src->draw(0,0,inw,inh);
    blurY->end();
    syphonTex.dst->end();
    syphonTex.swap();
    ofPopMatrix();
    ofPopStyle();
    ofPopView();
}

#endif


void VisuHandler::update(){
    paramSync.update();
#ifdef syphon
    blurblob();
#endif
    
    for(int i = 0;i<visuList.size();i++){
        if(visuList[i]->screenN>=0&&!visuList[i]->isHighFPS){
            ofRectangle curS = sH.rectOfScreen(visuList[i]->screenN);
        visuList[i]->update(curS.width,curS.height);
        }
    }

    
}


void VisuHandler::updateHighFPS(){
    paramSync.update();
    
    for(int i = 0;i<visuList.size();i++){
        if(visuList[i]->screenN>=0&&visuList[i]->isHighFPS){
            ofRectangle curS = sH.rectOfScreen(visuList[i]->screenN);
            visuList[i]->update(curS.width,curS.height);
        }
    }
    
    
}


void VisuHandler::saveState(string & s){
        if(s!=""){
    string abspath = ofToDataPath("presets/"+ofToString(loadName));
    if(s.find("/")!=string::npos) {abspath = s;}
    else{ofLogWarning("saving to local : " + abspath);}
    cout<<"saving to " + abspath<<endl;
    ofXml xml;
	xml.serialize(allParams);
	cout<<xml.save(abspath)<<endl;
        }
        else{
            ofLogWarning("no argument for save state");
        }

}





void VisuHandler::loadState(string & s){
    if(s!=""){
    string abspath = ofToDataPath("presets/"+ofToString(loadName));
    if(s.find("/")!=string::npos) {abspath = s;}
    else{ofLogWarning("loading from local : " + abspath);}
    ofXml xml;

	xml.load(abspath);
    xml.deserialize(allParams);
    }
    else{
        ofLogWarning("no argument for load state");
    }

}

ofImage * VisuHandler::getSharedImg(int i){
    if(i>sharedImg.size())return;
    return &sharedImg[i];
}
void VisuHandler::registerParams(){
    sH.registerParams();
    allParams.add((&sH)->screensParam);
    for(int i = 0 ; i< visuList.size();i++){
        visuList[i]->registerParam();
        allParams.add(visuList[i]->settings);
    }
    
    saveName.setName("saveName");
    allParams.add(saveName);
    loadName.setName("loadName");
    allParams.add(loadName);
    blobBlur.setName("blobBlur");
    blobBlur.setMin(0);
    blobBlur.setMax(10);
    blobBlur = 0;
    allParams.add(blobBlur);
    attr->registerParam();
    allParams.add(attr->settings);
    
#ifdef GUIMODE
    paramSync.setup(allParams,VISU_OSC_IN,VISU_OSC_IP_OUT,VISU_OSC_OUT);
#else
    paramSync.setup(allParams,VISU_OSC_OUT,"0",VISU_OSC_IN);
#endif
    
    saveName.addListener(this, &VisuHandler::saveState);
    loadName.addListener(this, &VisuHandler::loadState);
    
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
   
    glBlendEquation(GL_FUNC_ADD_EXT);
    
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA);

    for (int i = 0 ; i<visuList.size();i++){
        ofPushMatrix();
        ofPushView();
        ofPushStyle();
        ofTranslate(0,0,zdepth/2);
        if(visuList[i]->screenN>=0){
            int validScreen = sH.getValidScreen(visuList[i]->screenN);
            if(validScreen>=0){
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
    for (int i = 0 ; i< sH.screenL.size();i++){
        sH.screenL[i]->calcRectMax();
    }
}





