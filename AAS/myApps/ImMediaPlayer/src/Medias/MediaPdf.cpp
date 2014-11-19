//
//  MediaPdf.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaPdf.h"


void MediaPdf::load(string fp){
    pdf.load(fp);
    format = pdf.width*1.0/pdf.height;
    isLoaded = true;
}

void MediaPdf::drawMedia(){
    ofPushMatrix();
    ofSetColor(255);
    ofDrawRectangle(drawBox);
    ofTranslate(drawBox.x,drawBox.y);
    pdf.draw();
    ofPopMatrix();
}

void MediaPdf::boxResized(bool stable){
    Media::boxResized(stable);
    pdf.resize(drawBox.width, drawBox.height);
}


void MediaPdf::offset(ofVec2f t){
 
    
}


