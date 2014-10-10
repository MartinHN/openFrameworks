//
//  jsonLoader.h
//  ViZa
//
//  Created by martin hermant on 09/10/14.
//
//

#ifndef __ViZa__jsonLoader__
#define __ViZa__jsonLoader__

#include <iostream>
//#include "stdio.h"
#include "Container.h"
#include "ofxJSONElement.h"
#include "ofxCsv.h"
#include "yamlinput.h"

class jsonLoader{
 public:
    
    static int globalCount;
    
    static jsonLoader * instance();
    
    void loadSegments(string audiopath="",string segpath="");

    
    
private:
    static jsonLoader * inst;
    
};

#endif /* defined(__ViZa__jsonLoader__) */
