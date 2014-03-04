#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    
    Screen src;
    
    src = Screen(ofPoint(100,200), ofPoint(200,600),ofPoint(200, 700), ofPoint(100,700));
    listOfScreen.push_back(src);
    
    src = Screen(ofPoint(100,100), ofPoint(200,100),ofPoint(200, 200), ofPoint(100,200));
    listOfScreen.push_back(src);
    
    src = Screen(ofPoint(900,100), ofPoint(950,100),ofPoint(950, 500), ofPoint(900,500));
    listOfScreen.push_back(src);
    
    src = Screen(ofPoint(900,600), ofPoint(950,800),ofPoint(950, 800), ofPoint(900,800));
    listOfScreen.push_back(src);
    
    src = Screen(ofPoint(500,100), ofPoint(550,100),ofPoint(550, 300), ofPoint(500,300));
    listOfScreen.push_back(src);
    
    activeScreen = -1;
    
    gui.setup();
    suppr.addListener(this, &testApp::deleteScreen);
    add.addListener(this, &testApp::addScreen);
    reset.addListener(this, &testApp::resetScreen);
    save.addListener(this, &testApp::savePng);
    
    gui.add(suppr.setup("Suppr", 0));
    gui.add(add.setup("Add", 0));
    gui.add(reset.setup("Reset", 0));
    gui.add(save.setup("Save", 0));
    
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(0);
    gui.draw();
    
    for(int i=0; i<listOfScreen.size(); i++){
        
        listOfScreen.at(i).draw();

        
    }
    

    
    
}

//--------------------------------------------------------------
void testApp::deleteScreen(){
    
    if(activeScreen >= 0){
        listOfScreen.erase(listOfScreen.begin()+activeScreen);}
}

//--------------------------------------------------------------
void testApp::addScreen(){
   
    ofPoint p1 = ofPoint( ofGetScreenWidth()/2 - ofRandom(100), ofGetScreenHeight()/2 - ofRandom(100));
    ofPoint p2 = ofPoint( ofGetScreenWidth()/2 + ofRandom(100), ofGetScreenHeight()/2 - ofRandom(100));
    ofPoint p3 = ofPoint( ofGetScreenWidth()/2 + ofRandom(100), ofGetScreenHeight()/2 + ofRandom(100));
    ofPoint p4 = ofPoint( ofGetScreenWidth()/2 - ofRandom(100), ofGetScreenHeight()/2 + ofRandom(100));
    Screen src = Screen(p1, p2, p3, p4);
    
    listOfScreen.push_back(src);
    
}

//--------------------------------------------------------------
void testApp::resetScreen(){
    
    if(activeScreen>=0){
        
        listOfScreen.at(activeScreen).reset();
        
    }
    
}

//--------------------------------------------------------------
void testApp::savePng(){
    
    ofSaveScreen("mask.png");
    
    saveXml();
    
    
    
    
}

//--------------------------------------------------------------
void testApp::saveXml(){
    
    xml.clear();
    xml.addTag("ecranList");   
    xml.pushTag("ecranList");
    
    
    for(int i=0; i<listOfScreen.size(); i++)
    {
        
        Screen src = listOfScreen.at(i);
        
        xml.addTag(ofToString("ecran").append(ofToString(i)));
        xml.pushTag(ofToString("ecran").append(ofToString(i)));
       
        
        
        for (int j=0; j<src.listOfPoint.size(); j++)
        {
            
            
            xml.addTag(ofToString("point").append(ofToString(j)));
            xml.pushTag(ofToString("point").append(ofToString(j)));
            
            
            xml.setValue("x", src.listOfPoint.at(j).x);
            xml.setValue("y", src.listOfPoint.at(j).y);
            

            
            xml.popTag();
            
            
        }
        
        xml.popTag();
        
       
        
        
        
        
    }
    
    xml.popTag();
    xml.save("test");
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key) {
        case 'x':
            savePng();
            break;
        case 's':
            ofSetFullscreen(true);
            break;
            
        default:
            break;
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    ofPoint mouse = ofPoint(x,y);
    
    if(activeScreen>=0){
        
        listOfScreen.at(activeScreen).dragPoint(mouse);
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
   
    if( button == 0 ){
    
        ofPoint mouse = ofPoint(x,y);
        ofPolyline poly;
        poly.clear();
        bool activeOther = true;

        
        //Be sure the screen is not already active
        if(activeScreen>=0)
        {
        
            for(int j=0; j<(listOfScreen.at(activeScreen).listOfPoint.size()); j++)
            {
                
                poly.lineTo(listOfScreen.at(activeScreen).listOfPoint.at(j));
            }
            
            if(poly.inside(mouse)){
                
                activeOther = false;
                listOfScreen.at(activeScreen).findClosest(mouse);
            }
            else
            {
                activeOther = true;
                
                
            }
            
        }
        
        
        
        if(activeOther)
        {
            activeScreen = -1;
            poly.clear();
            for(int i=0; i<listOfScreen.size(); i++)
            {
                
                for(int j=0; j<(listOfScreen.at(i).listOfPoint.size()); j++)
                {
                    
                    poly.lineTo(listOfScreen.at(i).listOfPoint.at(j));
                }
                poly.close();
                
                if( poly.inside(mouse))
                {
                    activeScreen = i;
                    break;                
                }

                poly.clear();
                
            }
            
            for(int i=0; i<listOfScreen.size(); i++)
            {
            
                if( activeScreen == i)
                {
                    listOfScreen.at(i).isActive = true;
                    listOfScreen.at(i).findClosest(mouse);
                }
                else
                {
                    listOfScreen.at(i).isActive = false;
                    listOfScreen.at(i).activePoint = -1;
                }
                

                
            }
            
        }
        
    }
    

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
