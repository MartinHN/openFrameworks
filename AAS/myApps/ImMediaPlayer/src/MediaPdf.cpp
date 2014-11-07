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
    format = pdf.getWidth()*1.0/pdf.getHeight();
}

void MediaPdf::draw(){
    ofSetColor(255);
    ofRect(box);
    
    pdf.draw();
}

void MediaPdf::resize(int x, int y){
    pdf.resize(box.width, box.height);
}

void MediaPdf::offset(ofVec2f t){
 
    
}


