//
//  BlobHandler.h
//  MurMur d'Ombre
//
//  Created by martin hermant on 17/02/14.
//
//

#pragma once


#include "ofxOpenCv.h"
#include "ofxSyphon.h"
#include "Constants.h"
#include "ofMain.h"


class BlobHandler{
    
public:
    
    BlobHandler(){};
    
    
    void setup(int inwin, int inhin,ofShader* blurXin,ofShader * blurYin);
    
    void computePoly();
    void update();
    void blurblob();
    void getBlob();
    void registerParams();
    vector<ofVec3f> getCentroids(float w=1,float h=1);
    vector<ofPolyline> getBlobs(float w=1,float h=1);
    vector<ofVec3f> getExtrems(float w, float h);
    
    
    int inw,inh;
    
    
    ofxSyphonClient blobClient;
    pingPongBuffer syphonTex;

    ofShader * blurX;
    ofShader * blurY;
    ofShader threshBW;
    ofxCvContourFinder contourFinder;
    ofPixels pix;

    ofxCvGrayscaleImage gs;
    
    
        vector<ofxCvBlob> blobs;

    ofParameter<float> vidThreshold;
    ofParameterGroup settings;
    ofParameter<float> blobBlur;
    
    ofParameter<float> minSide, maxSide,maxLengthExtrem;
    ofParameter<int> maxBlobs,polyMaxPoints;
    ofParameter<bool> findHoles,invertBW;
    ofParameter<float> simplification;
};

