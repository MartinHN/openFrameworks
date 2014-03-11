//
//  Photo.cpp
//  MurMur d'Ombre
//
//  Created by Conil Aurélien on 07/03/2014.
//
//

#include "Photo.h"

Photo::Photo(VisuHandler * v):VisuClass(v){
    
    settings.setName("Photo");
    
    MYPARAM(numPhoto,0,0,11);
    numPhoto.addListener(this,&Photo::changeImage);
    MYPARAM(isResize,false,false,true);
    isResize.addListener(this, &Photo::changeResize);
    MYPARAM(isMirror, false, false, true);
    isMirror.addListener(this, &Photo::changeMirror);
    MYPARAM(color, ofVec3f(255, 255,255), ofVec3f(0,0,0), ofVec3f(255,255,255));
    
    ofImage img;
    img.loadImage("images/temps/1.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/2.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/3.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/4.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/5.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/6.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/7.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/8.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/9.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/10.jpg");
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/11.jpg");
    listOfImage.push_back(img);
    
    imgToDraw = listOfImage.front();
    
    isSet = false;
    


}

void Photo::update(int w,int h){
    
    if(!isSet )
    {
    
        
        if(isResize)
        {
            
            imgToDraw.resize(w, h);
            
            
        }
        else
        {
            
            int finalHeight = h;
            float ratioPhoto = imgToDraw.height/imgToDraw.width;
            imgToDraw.resize(finalHeight, finalHeight*1.0f/ratioPhoto);
            
            if(imgToDraw.width> w)
            {
                float lengthToCrop = (finalHeight*1.0f/ratioPhoto) - w ;
                imgToDraw.crop( lengthToCrop/2.0f, 0, w, h );
                
            }
        
        
        }
        
        if(isMirror){
            imgToDraw.mirror(false, true);
        }
       // imgToDraw.setAnchorPercent(0.5f, 0.5f);
        isSet = true;
        
    }
    
    
}

void Photo::draw(int w, int h){
    
    if(isSet && numPhoto>0){
        
        ofSetColor(color->x, color->y, color->z);
        imgToDraw.draw(0, 0, w, h);
    }
    
    
}

void Photo::changeImage(int& num){
    
    isSet = false;
    if( num< listOfImage.size()-1 && num>0){
    imgToDraw = listOfImage.at(num+1);
    }

    
}

void Photo::changeResize(bool& resize){
    
    isSet = false;
    int num = numPhoto;
    changeImage(num);
}

void Photo::changeMirror(bool &ismir){
    
    int num = numPhoto;
    changeImage(num);
    
    isSet = false;
    
}

