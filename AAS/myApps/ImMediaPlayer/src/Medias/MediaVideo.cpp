//
//  MediaVideo.cpp
//  ImMedia
//
//  Created by Tinmar on 28/10/2014.
//
//

#include "MediaVideo.h"


void MediaVideo::load(string filePath){

    bool Loaded = player.loadMovie(filePath);
    player.setLoopState(OF_LOOP_NONE);
    
    playImage.loadImage("Medias/playLogo.png");
    alphaPlay = 0;
    stopImage.loadImage("Medias/stopLogo.png");
    alphaStop =0;
    pauseImage.loadImage("Medias/pauseLogo.png");
    alphaPause = 0;
//    player.play();
    
    

}


void MediaVideo::update(){


    if(!loaded && player.isLoaded()){
        
        format = player.getWidth()*1.0/player.getHeight();

        loaded = true;
        player.setPaused(false);
        updateDrawBox();

    }
    player.update();
}

void MediaVideo::draw(){
    if(loaded){
    player.draw(drawBox.x,drawBox.y,drawBox.width,drawBox.height);

    }
    ofSetColor(255,alphaPlay);
    playImage.draw(drawBox);
    
    ofSetColor(255,alphaPause);
    pauseImage.draw(drawBox);
    
    ofSetColor(255,alphaStop);
    stopImage.draw(drawBox);
    

}


void MediaVideo::resize(int x, int y){

}


void MediaVideo::setTime(float pct){
    player.setPosition(pct);
}



void MediaVideo::touch(TouchType t,TouchAction a){
    GloveInteractBox::touch(t, a);
    if(t == GLOVE_CLICK){
        if(a==GLOVE_SHORTPRESS){
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
        else if ( a == GLOVE_LONGPRESS)
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
