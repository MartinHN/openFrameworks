//
//  BlobHandler.cpp
//  MurMur d'Ombre
//
//  Created by martin hermant on 17/02/14.
//
//



#include "BlobHandler.h"


void BlobHandler::setup(int inwin, int inhin,ofShader* blurXin,ofShader * blurYin,ScreenHandler * sHin){
    inw = inwin;
    inh = inhin;
    blurX=blurXin;
    blurY = blurYin;
    
    sH = sHin;
    

    blobClient.setup();
    //blobClient.setApplicationName("kinectExampleDebug");
    //blobClient.setServerName("blob");
    blobClient.setApplicationName("Quartz Composer");
     blobClient.setServerName("N");

    syphonTex.allocate(inw,inh,GL_RGB32F);
    
    gs.allocate(inw, inh);
 
    

    pix.allocate(inw,inh,3);

    
}


void BlobHandler::update(){
    
    compBlob();
    compCache();
    arms = compExtrems();
    centroids = compCentroid();
    boxes = compBounds();

    
//    blurblob();

}

void BlobHandler::registerParams(){
    settings.setName("blobsettings");

    MYPARAM(isPiping,false,false,true);
    MYPARAM(vidThreshold, 70.f, 0.f, 255.f);
    MYPARAM(invertBW,true,false,true);
    MYPARAM(minSide,0.f,0.f,1.f);
    MYPARAM(maxSide,1.f,0.f,1.f);
    MYPARAM(maxBlobs, 1,0,4);
    MYPARAM(findHoles,false,false,true);
    MYPARAM(simplification,0.f,0.f,50.f);
    MYPARAM(smooth,0.f,0.f,10.f);
    MYPARAM(polyMaxPoints, 0,0,200);
    MYPARAM(maxLengthExtrem, 15.f,0.f,100.f);
    MYPARAM(screenN, 0, 0, 123);
    MYPARAM(invertX,false,false,true);
    MYPARAM(invertY,false,false,true);
    MYPARAM(pos, ofVec3f(0.5),ofVec3f(0),ofVec4f(1.));
    MYPARAM(scale, ofVec3f(0.5),ofVec3f(0),ofVec4f(1.));
    MYPARAM(crop, ofVec4f(0),ofVec4f(0),ofVec4f(100));
}



//void BlobHandler::computePoly(){
//    
//    syphonTex.dst->begin();
//    threshBW.begin();
//    threshBW.setUniformTexture("tex",blobClient.getTexture(),1);
//    
//    
//    threshBW.end();
//    syphonTex.dst->end();
//    
//    ofxCvGrayscaleImage bw ;
//    bw.getTextureReference() = syphonTex.src->getTextureReference();
//    bw.threshold(40);
//
//}




void BlobHandler::compBlob(){
    glBlendEquation(GL_FUNC_ADD_EXT);
    
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA);
    
    syphonTex.dst->begin();
    ofSetColor(255);
    blobClient.draw(-crop->x,-crop->y,inw+crop->x+crop->z,inh+crop->y+crop->w);
    syphonTex.dst->end();
    
    //    syphonTex.swap();
    
    ofPixels pix;
    pix.allocate(inw,inh,3);
    
    
    
    
    syphonTex.dst->readToPixels(pix);
    
    ofxCvColorImage colorIm;
    colorIm.allocate(inw,inh);
    colorIm.setFromPixels(pix);
    colorIm.updateTexture();
    gs = colorIm;
    gs.threshold(vidThreshold,invertBW);
    
    contourFinder.findContours(gs, minSide*inw*inh*minSide, maxSide*inw*inh*maxSide, maxBlobs, findHoles);
   
    blobs = contourFinder.blobs;
    

    
    
}


vector<ofVec3f> BlobHandler::compCentroid(float w, float h){
    vector<ofVec3f> res;
    ofVec3f scale(w*1.0/lastw,h*1.0/lasth);
    for (int i = 0 ; i< blobs.size();i++){
       
        
        res.push_back(cachedP[i].getCentroid2D()/scale);
    }
    return res;
}

vector<ofRectangle> BlobHandler::compBounds(float w, float h){
    vector<ofRectangle> res;

    for (int i = 0 ; i< blobs.size();i++){
        ofRectangle rr = cachedP[i].getBoundingBox();
        
        res.push_back(ofRectangle(rr.x*w/lastw,rr.y*h/lasth,rr.width*w/lastw,rr.height*h/lasth));
    }
    return res;
}

void BlobHandler::compCache(){
    cachedP.clear();
    ofRectangle rr = sH->rectOfScreen(sH->getValidScreen(screenN));
    rr.scaleFromCenter(lastw/(*sH->scrw), lasth/(*sH->scrh));
    
    for (int i = 0 ; i< blobs.size();i++){
        ofPolyline pp ;
        
        for(int j = 0 ; j < blobs[i].nPts;j++){
            ofPoint p = blobs[i].pts[j]/ofVec3f(inw,inh);
            if(invertX)p.x = 1-p.x;
            if(invertY)p.y = 1-p.y;
            p  =  ofVec2f(0.5) + (p-ofVec2f(0.5)) * scale.get();
            p +=  (pos.get()-ofVec2f(0.5));
            
            
            
            p.x = ofMap(p.x, 0, 1, rr.getMinX() ,rr.getMaxX());
            p.y =ofMap(p.y, 0, 1, rr.getMinY() ,rr.getMaxY());
            p.z-=0.5;
            pp.lineTo(p);
        }
        
        if(polyMaxPoints>0){pp=pp.getResampledByCount(polyMaxPoints);}
        if(simplification>0){
            pp.simplify(simplification);
            
        }
        if(smooth>0){
            pp = pp.getSmoothed(smooth);
        }
        if(pp.size()>0)
            cachedP.push_back(pp);
    }

    
}

vector<ofPolyline> BlobHandler::getBlobs(float w, float h,bool invx,bool invy){

    if(lastw ==w && lasth == h){
        return cachedP;
    }
    else{
        vector<ofPolyline> res;
        ofVec2f ts = ofVec2f(w*1.0/lastw ,h*1.0/lasth);
        polyCacheDirty = false;
        for(int i = 0 ; i< cachedP.size() ; i++){
            ofPolyline l;
            for (int  j = 0 ; j< cachedP[i].size() ; j++){
                ofPoint lp;
                lp.x = !invx? cachedP[i][j].x : lastw-cachedP[i][j].x;
                lp.y = !invy ? cachedP[i][j].y :  lasth-cachedP[i][j].y;
                lp *= ts;
                l.lineTo(lp);
            }
            res.push_back(l);
        
        }
        lastw = w;
        lasth = h;
return res;
    }
    
    
}

vector<ofPath> BlobHandler::getPaths(float w, float h,bool invx,bool invy){
    vector<ofPath> res;
    vector<ofPolyline> p;

        p = getBlobs(w,h,invx,invy);
    
    for (int i = 0 ; i< p.size();i++){
        ofPath pp;
        
        for(int j = 0 ; j < p[i].size();j++){
            pp.lineTo(p[i][j]);
        }
        pp.close();
        if(p[i].size()>0)
            res.push_back(pp);
    }
    return res;
}

vector<ofVec3f> BlobHandler::compExtrems(float w, float h){
    vector<ofVec3f> res;
    vector<ofPolyline> tmp ;


    
    
    float sum_angles=0;
    int begin=0,end=1;
    float maxangle = 110;


    for (int i = 0 ; i< cachedP.size();i++){
        begin=0;
        end=1;
        sum_angles=0;
        ofPolyline tmpspaced = cachedP[i];//.getSmoothed(0.01);
//        tmpspaced = tmpspaced.getResampledBySpacing(0.01);
        deque<float> tmpp;

        while(end<tmpspaced.size()+maxLengthExtrem&&maxLengthExtrem<tmpspaced.size()){
            if(end-begin<=maxLengthExtrem){
                tmpp.push_back(tmpspaced.getRotationAtIndex(end%tmpspaced.size()).z>0?+tmpspaced.getAngleAtIndex(end%tmpspaced.size()):-tmpspaced.getAngleAtIndex(end%tmpspaced.size()));
                sum_angles+=tmpp.back();
                end++;
            }
            else if(tmpp.size()>0){
                sum_angles-=tmpp.front();
                tmpp.pop_front();
                
                tmpp.push_back(tmpspaced.getRotationAtIndex(end%tmpspaced.size()).z>0?+tmpspaced.getAngleAtIndex(end%tmpspaced.size()):-tmpspaced.getAngleAtIndex(end%tmpspaced.size()));
                sum_angles+=tmpp.back();
                
                begin++;
                end++;
            }
            else{break;}
            
            if(sum_angles>maxangle){
                float idx = begin;
                float sum = 0;
                for (int j = 0 ; j < tmpp.size() ; j++){
                    sum+= tmpp[j]*j;
                    }
                sum/=sum_angles;
                idx+=sum;
                if(idx>tmpspaced.size())idx-tmpspaced.size();
                ofPoint p =tmpspaced.getPointAtIndexInterpolated(idx);
                if(abs((p-tmpspaced.getCentroid2D()).getNormalized().dot(ofVec2f(1,0)))>cos(ofDegToRad(80))){
                
                res.push_back(p*ofVec2f(w*1.0/lastw,h*1.0/lasth));
                }
                
//                    if(test == false)ofLogWarning("no extrem");
                begin = end;
                end = begin;
                sum_angles=0;
                tmpp.clear();
            }
        }
    }
    

    return res;
}

//void BlobHandler::blurblob(){
//    ofPushMatrix();
//    ofPushStyle();
//    ofPushView();
//    
//    glBlendEquation(GL_FUNC_ADD_EXT);
//    
//    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA);
//    gs.dilate_3x3();
//    
//    syphonTex.dst->begin();
//    ofSetColor(255);
//    blurX->begin();
//    blurX->setUniform1f("blurAmnt", blobBlur);
//    gs.draw(0,0,inw,inh);
//    blurX->end();
//    syphonTex.dst->end();
//    glFlush();
//    
//    syphonTex.swap();
//    
//    syphonTex.dst->begin();
//    blurY->begin();
//    blurY->setUniform1f("blurAmnt", blobBlur);
//    syphonTex.src->draw(0,0,inw,inh);
//    blurY->end();
//    syphonTex.dst->end();
//    syphonTex.swap();
//    ofPopMatrix();
//    ofPopStyle();
//    ofPopView();
//}


