//
//  ofxPDF2.h
//  ImMedia
//
//  Created by martin hermant on 19/11/14.
//
//

#ifndef __ImMedia__ofxPDF2__
#define __ImMedia__ofxPDF2__

#include "mupdf/fitz.h"
#include "ofMain.h"


class ofxPDF2{
    public :
    
    ofxPDF2(){};
    float width;
    float height;
    
    void load(string str){char * fn = new char[str.size() + 1];
        std::copy(str.begin(), str.end(), fn);
        _load(fn);};
    void _load(char* fn);
   void close(){};
    
   void draw(){};
    
   void resize(int x, int y){};
    
};

#endif /* defined(__ImMedia__ofxPDF2__) */
