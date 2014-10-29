//
//  GButton.cpp
//  ImMedia
//
//  Created by Tinmar on 29/10/2014.
//
//

#include "GButton.h"

GButton::GButton() {
//    GloveInteract::GloveInteract();
    isDraggable =false;
    image.loadImage("UI/screenAnchor.png");
    name = "dd";
    box = ofRectangle();
    
}


GButton::GButton(ofVec2f center, ofVec2f size,const string & impath,const string & _name) {

    name = _name;
    isDraggable =false;
    image.loadImage(impath);
//    GloveInteract::GloveInteract();
    box = ofRectangle();
    box.setFromCenter(center,size.x,size.y);
}



void GButton::draw(ofEventArgs & e){
    image.width =box.width;
    image.height =box.height;
//    ofScale(box.width*1.0/image.width, box.height*1.0/image.height);
    image.draw(box.x, box.y);

}

void GButton::clicked(Cursor* gId,touchType & state){
    
};


