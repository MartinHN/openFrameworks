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
    MediaPdf();
    ~MediaPdf();
  
    
    ofxPDF pdf;
  
   void load(string filePath);
   void unLoad();
   void draw();
    void offset(ofVec2f t);
};


#endif /* defined(__ImMedia__MediaPdf__) */
