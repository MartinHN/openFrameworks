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
}

void MediaPdf::draw(ofEventArgs & a){
    ofSetColor(255);
    ofRect(box);
    pdf.draw(box);
}


void MediaPdf::offset(ofVec2f t){
 
    
}

void MediaPdf::unLoad(){
    pdf.close();
}
