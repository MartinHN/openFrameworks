//
//  MediaVideo.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaVideo.h"


void MediaVideo::load(string filePath){
    
    player.load(filePath);
    
    
    playImage.load("Medias/playLogo.png");
    
    alphaPlay = 0;
    stopImage.load("Medias/stopLogo.png");
    alphaStop =0;
    pauseImage.load("Medias/pauseLogo.png");
    alphaPause = 0;
    logoRect.set(0,0,1,1);
    

    
}


void MediaVideo::update(){
    
    
    if(!isLoaded && player.isLoaded()){
        
        format = player.getWidth()*1.0/player.getHeight();
        
        isLoaded = true;
        player.setPaused(true);
        updateDrawBox();
        
    }
    player.update();
}

void MediaVideo::drawMedia(){
    if(isLoaded){
//        player.draw(drawBox.x,drawBox.y,drawBox.width,drawBox.height);
        ofVec2f ratio = ofVec2f(player.width/drawBox.width,player.height/drawBox.height);
        if(subSection.width>0 && subSection.height>0){
        player.getTexture().drawSubsection(subSection.x, subSection.y,subSection.width, subSection.height,(subSection.x - drawBox.x)*ratio.x,(subSection.y - drawBox.y)*ratio.y,subSection.width*ratio.x,subSection.height*ratio.y);
        }
        
        ratio = ofVec2f(playImage.width/drawBox.width,playImage.height/drawBox.height);
        ofSetColor(255,alphaPlay);
        playImage.drawSubsection(subSection.x, subSection.y,subSection.width, subSection.height,(subSection.x - drawBox.x)*ratio.x,(subSection.y - drawBox.y)*ratio.y,subSection.width*ratio.x,subSection.height*ratio.y);
        
        ofSetColor(255,alphaPause);
        pauseImage.drawSubsection(subSection.x, subSection.y,subSection.width, subSection.height,(subSection.x - drawBox.x)*ratio.x,(subSection.y - drawBox.y)*ratio.y,subSection.width*ratio.x,subSection.height*ratio.y);
        
        ofSetColor(255,alphaStop);
        stopImage.drawSubsection(subSection.x, subSection.y,subSection.width, subSection.height,(subSection.x - drawBox.x)*ratio.x,(subSection.y - drawBox.y)*ratio.y,subSection.width*ratio.x,subSection.height*ratio.y);
    }
    
}




void MediaVideo::boxMoved(bool stable){
    Media::boxMoved(stable);
    logoRect.scaleTo(box, OF_SCALEMODE_FIT);
}

void MediaVideo::boxResized(bool stable){
    Media::boxResized( stable);
    logoRect.scaleTo(box, OF_SCALEMODE_FIT);
}




void MediaVideo::setTime(float pct){
    player.setPosition(pct);

}



void MediaVideo::touch(TouchButton t,TouchAction a){
    GloveInteractBox::touch(t, a);
    if(t == GLOVE_BUTTON_CLICK && isHovered && wasSelected && isSelected){
        if(a==GLOVE_ACTION_SHORTPRESS){
            if(!player.isPlaying()){
                player.play();
                alphaPlay = 255;
                Tweener.addTween(alphaPlay, 0, 5);
            }
            {
                player.setPaused(!player.isPaused());
                if(player.isPaused()){
                    Tweener.removeTween(alphaPlay);
                    alphaPlay=0;
                    alphaPause = 255;
                    Tweener.addTween(alphaPause, 0, 5);
                }
                else{
                    Tweener.removeTween(alphaPause);
                    alphaPlay=255;
                    alphaPause = 0;
                    Tweener.addTween(alphaPlay, 0, 5);
                }
                
            }
        }
        else if ( a == GLOVE_ACTION_LONGPRESS)
        {
            Tweener.removeTween(alphaPause);
            Tweener.removeTween(alphaPlay);
            alphaPlay=0;
            alphaPause = 0;
            alphaStop=255;
            Tweener.addTween(alphaStop, 0, 5);
            player.stop();
        }
    }
    
    
    
}
