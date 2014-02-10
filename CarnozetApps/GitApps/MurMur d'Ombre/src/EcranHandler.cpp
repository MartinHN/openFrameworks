//
//  EcranHandler.cpp
//  MurMur d'Ombre
//
//  Created by Tinmar on 06/02/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "EcranHandler.h"


ScreenHandler::ScreenHandler(){
screensParam.setName("screens");

    
    
}


ScreenHandler::ScreenHandler(int win, int hin, int zin){
    scrw=win;
    scrh = hin;
    zdepth=zin;
    screensParam.setName("screens");
    
#ifdef LIVEBLUR
    blurX.load("","shaders/blurXa.frag");
    blurY.load("","shaders/blurYa.frag");
    blurX.setUniform1f("blurAmnt", 10);
    blurY.setUniform1f("blurAmnt", 10);
    blur.allocate(scrw,scrh,GL_RGB32F);
    float * pos = new float[scrw*scrh*3];
    for (int x = 0; x < scrw; x++){
        for (int y = 0; y < scrh; y++){
            int i = scrw * y + x;
            
            pos[i*3 + 0] = 0;
            pos[i*3 + 1] = 0;
            pos[i*3 + 2] = 0;
        }
    }
    
    // Load this information in to the FBO´s texture
    blur.src->getTextureReference().loadData(pos, scrw,scrh, GL_RGB);
    blur.dst->getTextureReference().loadData(pos, scrw,scrh, GL_RGB);
    delete pos;
#endif
    
}

void ScreenHandler::setup(int win, int hin, int zin){
    scrw=win;
    scrh = hin;
    zdepth=zin;
    screensParam.setName("screens");
    
#ifdef LIVEBLUR
    blurX.load("","shaders/blurXa.frag");
    blurY.load("","shaders/blurYa.frag");
    blurX.setUniform1f("blurAmnt", 10);
    blurY.setUniform1f("blurAmnt", 10);
    blur.allocate(scrw,scrh,GL_RGB32F);
    float * pos = new float[scrw*scrh*3];
    for (int x = 0; x < scrw; x++){
        for (int y = 0; y < scrh; y++){
            int i = scrw * y + x;
            
            pos[i*3 + 0] = 0;
            pos[i*3 + 1] = 0;
            pos[i*3 + 2] = 0;
        }
    }
    
    // Load this information in to the FBO´s texture
    blur.src->getTextureReference().loadData(pos, scrw,scrh, GL_RGB);
    blur.dst->getTextureReference().loadData(pos, scrw,scrh, GL_RGB);
    delete pos;
#endif
    

}

void ScreenHandler::addScreen(vector<ofVec3f> vert){
    screenL.push_back(new Ecran(screenL.size(),vert,scrw,scrh));
}

void ScreenHandler::registerParams(){
    screensParam.clear();
    for(int i = 0 ;i< screenL.size();i++){
        screenL[i]->registerParams();
      screensParam.add(screenL[i]->vertices);  
    }
    
    screensParam.setName("screens");
   
    return screensParam;
}

ofRectangle ScreenHandler::rectOfScreen(int which){
int k = 1;
int idx = which%10;
ofRectangle res;

do{
    if(idx<screenL.size()){

        if(k==1)res = screenL[idx]->rectMax;
            else res.growToInclude(screenL[idx]->rectMax);
                }
    idx = (which/(k*10)) %10;
    k++;
}while(idx>0);

return res;

}

const ofVec2f ScreenHandler::sizeOfScreen(const int which){
    ofRectangle tmp = rectOfScreen(which);
    return ofVec2f(tmp.width,tmp.height);
}


void ScreenHandler::blurmask(){
#ifdef LIVEBLUR
//    blur.src->begin();
//
//    
//    blur.src->end();
    ofPushView();
    ofPushMatrix();
    ofPushStyle();
    blur.allocate(scrw,scrh,GL_RGB32F);
    blur.src->begin();
//    blurX.begin();
//    blurX.setUniform1f("blurAmnt",finalblur);
    ofSetColor(100);
    ofRect(0,0,scrw,scrh);
    ofSetColor(0,0, 255);
    
    ofRect(0, 0, 400, 400);
//    for(int z = 1 ; z<screenL.size();z++){
//        ofPolyline tmpPath(screenL[z]->getVertices());
//        tmpPath.close();
//        tmpPath.draw();
//    } 
//    blurX.end();
    blur.src->end();
//    blur.swap();
//    
//    blur.dst->begin();
//    blurY.begin();
////    blurY.setUniform1f("blurAmnt",finalblur);            
//    blur.src->draw(0,0);
//    blurY.end();        
//    blur.dst->end();
//    
//    blur.swap();
//    
//    
//    
//   
    ofPopView();
    ofPopMatrix();
    ofPopStyle();
#else
    //Create globalMask
    
    ofPixels pix;
    pix.allocate(scrw,scrh,OF_IMAGE_COLOR_ALPHA);
    
    ofVec2f scale(scrw,scrh);
    
    for(int i = 0 ; i < scrw;i++){
        for (int j =  0; j<scrh;j++){
            pix.setColor(i,j,ofColor(0,0,0,255)); 
            for(int z = 1 ; z<screenL.size();z++){
                ofPolyline tmpPath(screenL[z]->getVertices());
                tmpPath.close();
                if(tmpPath.inside(i,j)){
                    pix.setColor(i,j,ofColor(0,0,0,0));
                    //                    cout<<z<<endl;
                    break;
                }
            }
        }
    }
    int blurSize = 10;
    for(int i = blurSize ; i < scrw-blurSize;i++){
        for (int j =  0; j<scrh;j++){
            
            
            float a = pix.getColor(i+blurSize*1.0,j)[3]*0.06+
            pix.getColor(i+blurSize*0.75,j)[3]*0.09+
            pix.getColor(i+blurSize*0.5,j)[3]*0.12 +
            pix.getColor(i+blurSize*0.25,j)[3]*0.15 +
            pix.getColor(i,j)[3]*0.16 +
            pix.getColor(i-blurSize*1.0,j)[3]*0.06 +
            pix.getColor(i-blurSize*0.75,j)[3]*0.09 +
            pix.getColor(i-blurSize*0.5,j)[3]*0.12 +
            pix.getColor(i-blurSize*0.25,j)[3]*0.15 ;
            //                a*=pix.getColor(i,j)[3]*1.0;
            ofColor color = ofColor(pix.getColor(i,j)[0],pix.getColor(i,j)[1],pix.getColor(i,j)[2], ofClamp(a,0,255));
            pix.setColor(i,j,color);
            //                if(color[3]>0&&color[3]<255) cout<<color[3]<<endl;
        }
    }
    for(int i = blurSize ; i < scrw-blurSize;i++){
        for (int j =  blurSize; j<scrh-blurSize;j++){
            ofColor color = ofColor(pix.getColor(i,j)[0],pix.getColor(i,j)[1],pix.getColor(i,j)[2], (
                                                                                                     pix.getColor(i,j+blurSize*1.0)[3]*0.06 +
                                                                                                     pix.getColor(i,j+blurSize*0.75)[3]*0.09 +
                                                                                                     pix.getColor(i,j+blurSize*0.5)[3]*0.12 +
                                                                                                     pix.getColor(i,j+blurSize*0.25)[3]*0.15 +
                                                                                                     pix.getColor(i,j)[3]*0.16 +
                                                                                                     pix.getColor(i,j-blurSize*1.0)[3]*0.06 +
                                                                                                     pix.getColor(i,j-blurSize*0.75)[3]*0.09 +
                                                                                                     pix.getColor(i,j-blurSize*0.5)[3]*0.12 +
                                                                                                     pix.getColor(i,j-blurSize*0.25)[3]*0.15 ));
            pix.setColor(i,j,color);
        }
    }
    
    
    globalMask.setFromPixels(pix);
    
    pix.clear();

#endif
}


void ScreenHandler::drawMask(){
    ofSetColor(0);
    ofRect(0,0, scrw,scrh);
    ofSetColor(0,0,244);
    for (int i = 1 ; i < screenL.size();i++){
        ofPath tmpP;
        vector<ofVec3f> vert = screenL[i]->getVertices();
        for(int j = 0 ; j<vert.size();j++){
            tmpP.lineTo(vert[j]);
        }
        tmpP.close();
        tmpP.draw();
    }
    
}
void ScreenHandler::loadScreensPos(){
   
    vector<ofVec3f> vertglob;
    vertglob.push_back(ofVec2f(0,0));
    vertglob.push_back(ofVec2f(1,0));
    vertglob.push_back(ofVec2f(1,1));
    vertglob.push_back(ofVec2f(0,1));
    
    addScreen(vertglob);
    
    ofXml eS;
    if(eS.load("Xml/ecrans.xml")){
        int nS = eS.getNumChildren();
        vector<ofVec3f> vert;
        
        for (int n = 0 ; n<nS;n++){
            eS.setToChild(n);
            
            int nP = eS.getNumChildren();
            eS.setToChild(0);
            for (int i = 0 ; i<nP;i++){
                ofVec3f curpoint(0,0);
                curpoint.x = eS.getFloatValue("x");
                curpoint.y = eS.getFloatValue("y");
                curpoint.z = eS.getFloatValue("z");
                vert.push_back(curpoint); 
                eS.setToSibling();
            }
            
            addScreen(vert);
            eS.setToParent();
            eS.setToParent();
            vert.clear();
            
        }
    }
    else{
        
        cout<<"No screen Loaded from Xml"<<endl;
        
    }
    
    
    
    blurmask();    //delete tmpPath;
    
}

//
//void EcranHandler::saveFile(string name){
//    ofXml xml;
//    xml.serialize(dstPoint[0]);
//    xml.serialize(dstPoint[1]);
//    xml.serialize(dstPoint[2]);
//    xml.serialize(dstPoint[3]);
//    xml.save(name);
//    
//}

