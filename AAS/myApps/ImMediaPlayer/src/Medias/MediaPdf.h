//
//  MediaPdf.h
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#ifndef __ImMedia__MediaPdf__
#define __ImMedia__MediaPdf__

#include <stdio.h>
#include "Media.h"
#include "ofxPDF.h"

class MediaPdf:public  Media {
    
    
public:
    MediaPdf(){};
    ~MediaPdf(){pdf.close();};
  
    
    ofxPDF pdf;
  
    virtual void load(string filePath);
    virtual void draw();
    virtual void resize(int x, int y);
    void offset(ofVec2f t);
    
    ofRectangle lastbox;
};


#endif /* defined(__ImMedia__MediaPdf__) */
