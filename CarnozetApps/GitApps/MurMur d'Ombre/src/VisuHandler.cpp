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
VisuHandler::VisuHandler(AttrCtl *attrctl,int inwin,int inhin,int zdepthin, int scrwin, int scrhin){
           
    allParams.clear();
    allParams.setName("Visu");
    zdepth = zdepthin;
    inw=inwin;
    inh= inhin;
    scrw=scrwin;
    scrh=scrhin;
    beat=0;
    attr = attrctl;
    
#ifdef HOMOGRAPHY
    fbo.allocate(scrw,scrh,GL_RGBA32F);
#endif
    syphonTex.allocate(inw,inh,GL_RGBA32F);
    
    sH = ScreenHandler(scrw,scrh,zdepth);
    sH.loadScreensPos();
    
   
    

    
   
}
void VisuHandler::setup(AttrCtl *attrctl, int inwin, int inhin, int zdepthin, int scrwin, int scrhin){
    
    allParams.clear();
    allParams.setName("Visu");
    zdepth = zdepthin;
    inw=inwin;
    inh= inhin;
    scrw=scrwin;
    scrh=scrhin;
    beat=0;
    attr = attrctl;
    
#ifdef HOMOGRAPHY
    fbo.allocate(scrw,scrh,GL_RGBA32F);
#endif
    syphonTex.allocate(inw,inh,GL_RGBA32F);
    
    sH.setup(scrw,scrh,zdepth);
    sH.loadScreensPos();
    
    
    

}
VisuClass * VisuHandler::get(const string & name){
    for (int i = 0; i< visuList.size() ; i++){
        if(visuList[i]->settings.getName()==name) return visuList[i];
    }
    
    return NULL;  
}



void VisuHandler::update(){
    paramSync.update();
    
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
//    sH.screensParam.setSerializable(false);
    string abspath = ofToDataPath("presets/"+ofToString(s));
    cout<<"saving to " + abspath<<endl;
    ofXml xml;
	xml.serialize(allParams);
	cout<<xml.save(abspath)<<endl;
}
void VisuHandler::loadState(string & s){
    string abspath = ofToDataPath("presets/"+ofToString(loadName));
    cout<<"loading from " + abspath<<endl;
    
    ofXml xml;
//	xml.serialize(allParams);
	cout<<xml.load(abspath)<<endl;
    xml.deserialize(allParams);
//    ofFile filein(abspath,ofFile::ReadOnly,false);
////    ofBuffer
//    if(!filein.exists())return;
//    //    ostream streamout;
//    ofBuffer buf = filein.readToBuffer();
//    
//    ofParameterGroup curG = allParams;
//    while(!buf.isLastLine()){
//        string in  = buf.getNextLine();
//    vector<string> args = ofSplitString(in,":",true);
//        if(args.size()>1){
//            if(curG.contains(args[0]))curG[args[0]].fromString(args[1]);
//            else cout<<"error reading"<<endl;
//        }
//        else{
//            if(curG.contains(args[0])) curG =curG.getGroup(args[0]);
//            else curG = curG.getParent()->getGroup(args[0]);
//        }
//    
//    cout<<ofToString(args[0]) + ofToString(curG.contains(args[0]))<<endl;
//    }
////    filein>>allParams;
//    filein.close();
    
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
   

    for (int i = 0 ; i<visuList.size();i++){
        ofPushMatrix();
        ofPushView();
        ofPushStyle();
        ofTranslate(0,0,zdepth/2);
        if(visuList[i]->screenN>=0){
            ofRectangle curS = sH.rectOfScreen(visuList[i]->screenN);
            if(curS.width>0){
#ifdef HOMOGRAPHY
            if(visuList[i]->screenN<sH.screenL.size()){
                if(visuList[i]->isMapping)sH.screenL[visuList[i]->screenN]->screenWarp.loadMat();
                else ofTranslate(curS.x,curS.y,0);
                visuList[i]->draw(curS.width,curS.height);
                if(visuList[i]->isMapping)sH.screenL[visuList[i]->screenN]->screenWarp.unloadMat();
            }
            else{
                ofTranslate(curS.x,curS.y,0);
#endif  
                visuList[i]->draw(curS.width,curS.height);
        
#ifdef HOMOGRAPHY
        }
#endif
            }

        }
        ofPopStyle();
        ofPopView();
        ofPopMatrix();
    }
    

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






