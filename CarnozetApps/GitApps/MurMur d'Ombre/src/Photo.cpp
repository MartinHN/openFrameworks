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
    
    MYPARAM(numPhoto,0,0,10);
    numPhoto.addListener(this,&Photo::changeImage);
    MYPARAM(isResize,false,false,true);
    isResize.addListener(this, &Photo::changeResize);
    
    ofImage img;
    img.loadImage("images/temps/1.png");
    img.setAnchorPercent(0.5,0.5);
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/2.png");
    img.setAnchorPercent(0.5,0.5);
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/3.png");
    img.setAnchorPercent(0.5,0.5);
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/4.png");
    img.setAnchorPercent(0.5,0.5);
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/5.png");
    img.setAnchorPercent(0.5,0.5);
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/6.png");
    img.setAnchorPercent(0.5,0.5);
    listOfImage.push_back(img);
    
    img.loadImage("images/temps/7.png");
    img.setAnchorPercent(0.5,0.5);
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
        
        imgToDraw.setAnchorPercent(0.5f, 0.5f);
        isSet = true;
        
    }
    
    
}

void Photo::draw(int w, int h){
    
    if(isSet && numPhoto>0){
        
        imgToDraw.draw(w/2.0f, h/2.0f);
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
}

