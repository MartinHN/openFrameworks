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
#include "ofxPDF2.h"

class MediaPdf:public  Media {
    
    
public:
    MediaPdf(){Media::Media();type = PDF;};
    ~MediaPdf(){pdf.close();};
  
    
    ofxPDF2 pdf;
  
    virtual void load(string filePath);
    virtual void drawMedia();
    virtual void boxResized(bool stable);

    void offset(ofVec2f t);

};


#endif /* defined(__ImMedia__MediaPdf__) */
