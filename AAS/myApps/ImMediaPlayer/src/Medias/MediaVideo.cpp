//
//  MediaVideo.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaVideo.h"


void MediaVideo::load(string filePath){
    
    player.loadMovie(filePath);
    
    
    playImage.loadImage("Medias/playLogo.png");
    alphaPlay = 0;
    stopImage.loadImage("Medias/stopLogo.png");
    alphaStop =0;
    pauseImage.loadImage("Medias/pauseLogo.png");
    alphaPause = 0;

    
    
    
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
        player.draw(drawBox.x,drawBox.y,drawBox.width,drawBox.height);
        
        
        ofSetColor(255,alphaPlay);
        playImage.draw(logoRect);
        
        ofSetColor(255,alphaPause);
        pauseImage.draw(logoRect);
        
        ofSetColor(255,alphaStop);
        stopImage.draw(logoRect);
    }
    
}


void MediaVideo::boxMoved(){
    logoRect.set(box.getCenter(),playImage.width,playImage.height);
    logoRect.scaleTo(box, OF_SCALEMODE_FIT);
}

void MediaVideo::boxResized(){
    logoRect.set(box.getCenter(),playImage.width,playImage.height);
    logoRect.scaleTo(box, OF_SCALEMODE_FIT);
}




void MediaVideo::setTime(float pct){
    player.setPosition(pct);
}



void MediaVideo::touch(TouchButton t,TouchAction a){
    GloveInteractBox::touch(t, a);
    if(t == GLOVE_BUTTON_CLICK && isSelected){
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
