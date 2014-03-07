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
    MYPARAM(isResize,false,false,true);
    
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
    
    img.loadImage("images/temps/7.png")
    img.setAnchorPercent(0.5,0.5);
    listOfImage.push_back(img);
    
    isSet = false;
    


}

void Photo::update(w,h){
    
    if(!isSet)
    {
    
        imgToDraw = listOfImage.at(numPhoto);
        if(isResize)
        {
            
            imgToDraw.resize(int newWidth, int newHeight);
            
            
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
                
                img.crop(int x, int y, int w, int h);
                
            }
        
        
        }
        
        
    
        
        
    }
    
    
}

