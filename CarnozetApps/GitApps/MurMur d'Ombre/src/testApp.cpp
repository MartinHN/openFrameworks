#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
//    ofSetDataPathRoot("../Resources/data/");
    #ifndef GUIMODE
    
 
    isFPS = true;
    cropxr=cropxl=cropyh=cropyl=0;
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofImage tmp;
    tmp.loadImage("images/backgroundvid.png");
    background.push_back(tmp);
    tmp.loadImage("images/background2.png");
    background.push_back(tmp);
    tmp.loadImage("images/background3.png");
    background.push_back(tmp);
    tmp.loadImage("images/background4.png");
    background.push_back(tmp);
    tmp.loadImage("images/background5.png");
    background.push_back(tmp);
    for(int i = 0 ; i<background.size();i++){
        background[i].setAnchorPercent(.50,.50);
    }
    zoomback = 100;
    curzoomback=zoomback;
    zoombackalpha = 0.8;
    

    //delete(tmp);
    backgroundtype=0;
    foregroundtype=1;
    isFullScreen=false;



    ofBackground(0);
    glDisable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();
    glBlendEquation(GL_FUNC_ADD_EXT);
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    ofSetBackgroundAuto(false);
    ofEnableArbTex();
    glEnable(GL_POINT_SPRITE);
    glPointParameteri(	GL_POINT_SPRITE_COORD_ORIGIN,GL_UPPER_LEFT);

ofSetVerticalSync(false);
     ofSetFrameRate(FPS);
   
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    inw=320;
    inh=240;
    videow=320;
    videoh=240;

    //Zdepth
    zdepth=1000;    
    camera.setFov(50);//50
    camrot.x=camdest.x=1.0;
    camrot.y=camrot.z=camrot.w=0.0;
    camdest.y=camdest.z=camdest.w=0.0;
    camera.disableOrtho();
      
#ifdef testvid
    vidplay.loadMovie("bien.mov");
    vidplay.play();
    threshold=55;
#endif


  camwiggle=CamWiggler();
    

    receiver.setup(12345);
    
    
    
    blurX.load("","shaders/blurX.frag");
    blurY.load("","shaders/blurY.frag");
    colorMod.load("","shaders/colorMod.frag");
     
    
    finalRender.allocate(scrw,scrh,GL_RGB);
        

    drawcam=true;
    isAtt=false;

    
    
#if defined  localcamera || defined testvid || defined blobcomp

    colorImg.allocate(inw,inh);
   grayImage.allocate(inw,inh);
    grayDiff.allocate(inw,inh);

    bLearnBakground=true;
 
#endif  
    

    alphablur=100;
    finalblur=0;
    rback=gback=bback=255;


    attrctl = AttrCtl();  

    brightness2=saturation2=contrast2=brightness=contrast=saturation=1.;
#endif  

    visuHandler.setup(&attrctl,inw,inh,zdepth,scrw,scrh);

#ifndef GUIMODE 
    visuHandler.setupSyphon(&blurX,&blurY);
#ifndef LIVEBLUR
    foreground.push_back(visuHandler.sH.globalMask);
#endif
    
#ifdef blobosc
    drawBlob = false;
#endif
    
    polyBlob.allocate(inw,inh,GL_RGB32F);
//    Shadow.load("","shaders/Shadow.frag");
    bblob=rblob=gblob=255;
#endif
    
    visuHandler.addVisu(new boule2gomme(&visuHandler));
    visuHandler.addVisu(new Particles(&visuHandler));

    visuHandler.registerParams();
    string savename = "lolo";
    visuHandler.saveState(savename);
#ifdef GUIMODE
    
    gui.load(visuHandler.allParams);

    
#endif

}




//--------------------------------------------------------------

///// UPDATE

//---------------------------------------------------------------



void testApp::update(){
#ifndef GUIMODE
    visuHandler.update();
    for (int ncom = 0 ; ncom< computeRatio;ncom++){
        visuHandler.updateHighFPS();
    }

        
    oscUpdate();
  
   
    attrctl.smooth();
    attrctl.addpoints();
//    attrctl.setattr();
#endif 

    
} 


//--------------------------------------------------------------
//     
//     DRAW
//
//--------------------------------------------------------------











void testApp::draw(){
    
#ifdef GUIMODE
    ofSetBackgroundAuto(true);
    gui.draw();
    
#else
    
    
    glBlendEquation(GL_FUNC_ADD_EXT);
    
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA);

        alphacamrot=0.3;
        if(camctl>0){
            vector<ofPoint> camctlpoints = attrctl.getFamilly(camctl-1);
            if(camctlpoints.size()==2){
                camdest.w = (ofVec2f(1,0).angle(camctlpoints[1]-camctlpoints[0]));
            }
            else camdest.w=0;
        }
        
        if(abs(camrot.y-camdest.y)>100)camdest.y=0;
        //if(abs(camrot.w-camdest.w)>250)camdest.w= 0;
        if(abs(camrot.w-camdest.w)>100) camdest.w=-camdest.w;
        
        
        camrot = camdest*alphacamrot + camrot*(1-alphacamrot);
 //       camwiggle.update(visuHandler.beat);
 //    camera.orbit(camrot.y,camrot.z,camwiggle.z*height*camrot.x,ofVec3f(width/2+camwiggle.x,height/2 +camwiggle.y, zdepth/2));       
         camera.orbit(camrot.y,camrot.z,1.0*height*camrot.x,ofVec3f(width/2,height/2 , zdepth/2)); 

        camera.rotate(camrot.w,camera.getLookAtDir());

#ifdef OF_VERSION_MAJOR
        camera.setVFlip(true);
#endif


    finalRender.dst->begin();
    
    ofSetColor(rback,gback,bback,alphablur);
       if(backgroundtype>0)  {
           
           
           if(backTrack){
               for (int i = 0 ; i<attrctl.curp.size() ; i++){
                   if(attrctl.familly[i] == 0){offsetbkg = attrctl.curp[i]*ofPoint(scrw,scrh);}
               }
           }
           else{
               offsetbkg.set(scrw/2,scrh/2);
           }
           curzoomback = zoomback;//*(1-zoombackalpha) +curzoomback*zoombackalpha;
           background[backgroundtype-1].draw(offsetbkg.x,offsetbkg.y,scrw*curzoomback/100.,scrh*curzoomback/100.);
       }
       else{
        ofRect(0,0,scrw,scrh);
       }

    
    
        if(isAtt){
            for( int k=0;k<attrctl.addpoint.size() ; k++){
                ofSetColor(attrctl.addfamilly[k]==0?255:0,attrctl.addfamilly[k]==1?255:0,attrctl.addfamilly[k]==2?255:0);
                ofCircle(attrctl.addpoint[k].x*width,attrctl.addpoint[k].y*height,20);   
            }
            
        }
         
    
    
    
          camera.begin();

        if(drawBlob)   {
            
#ifdef blobosc
            
            ofPushStyle();
            ofPushMatrix();
            ofSetColor(ofColor(255,255,255,255));
            
            glBlendColor(rblob/255.0,gblob/255.0,bblob/255.0,ablob/255.0);
            glBlendFunc(GL_CONSTANT_COLOR, GL_ONE);
            
          if(drawBlob)polyBlob.src->getTextureReference().draw(0,0,zdepth/2,width,height);
            
            
            ofPopStyle();
            ofPopMatrix();
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); 
#endif   
            
        }   
        
        

        //NEW POSITION for VISU
        visuHandler.draw();
          
  camera.end();
        
        




finalRender.dst->end();
    
    finalRender.swap();
                  
            finalRender.dst->begin();
            blurX.begin();
            blurX.setUniform1f("blurAmnt",finalblur);
            finalRender.src->draw(0,0); 
            blurX.end();
            finalRender.dst->end();
            
            finalRender.swap();

            finalRender.dst->begin();
            blurY.begin();
            blurY.setUniform1f("blurAmnt",finalblur);            
            finalRender.src->draw(0,0);
            blurY.end();        
            finalRender.dst->end();
            
            finalRender.swap();

            finalRender.dst->begin();
            colorMod.begin();
            colorMod.setUniform1f("contrast",contrast); 
            colorMod.setUniform1f("saturation",saturation); 
            colorMod.setUniform1f("brightness",brightness); 
            finalRender.src->draw(0,0);
            colorMod.end();        
            finalRender.dst->end();
            
            finalRender.swap();
            
            
     
        
        


   
        
        glFinish();
      
       
                  

    
    ofSetColor(255);
    

    

    if(foregroundtype>0){
#ifdef LIVEBLUR
        visuHandler.sH.drawMask();
#else
        foreground[foregroundtype-1].draw(0,0,scrw,scrh);
#endif
    }
    ofSetColor(0);
    glBlendEquation(GL_ADD);
    glBlendFunc(GL_DST_COLOR,GL_ZERO);

    ofSetColor(255);
    finalRender.src->draw(cropxl,cropyh,ofGetWidth()-cropxr-cropxl,ofGetHeight()-cropyl-cropyh); 
    

    if(isFPS){
        ofSetColor(0,0,0,!isFPS?0:255);
        ofRect(0, 0, 300, 25);
        ofSetColor(255, 255, 255,!isFPS?0:255);
        ofDrawBitmapString("Fps: " + ofToString( ofGetFrameRate()) + "thr :" + ofToString(threshold), 15,15);
    }
    else{
        ofSetColor(0,0,0);
        ofRect(0, 00, 1000, 25);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Un Des Sens : www.undessens.org",50,40);
        ofDrawBitmapString(" www.facebook.com/AssoUnDesSens",500,40);
    }
    
    
 #endif
    
    
}
#ifdef GUIMODE
void testApp::keyPressed(int key){
     switch (key){
         case 's':
         {ofFileDialogResult filep = ofSystemSaveDialog("preset","save preset file");
             visuHandler.saveName = filep.getPath();}
             break;
         case 'l':
         {ofFileDialogResult filep = ofSystemLoadDialog("load preset");
             visuHandler.loadName = filep.getPath();}
             break;
     }
}
#endif


#ifndef GUIMODE
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    int k = 0;

    switch (key){


#ifdef blobcomp
            case 'b':
            bLearnBakground=true;
            break;
#endif

        case 's':
            isFullScreen=!isFullScreen;
            ofSetFullscreen(isFullScreen);
            break;
            
            
            
            case'a':
            isAtt=!isAtt;
            break;
            
            case'+':
            threshold+=1;
            break;
            
            case'-':
            threshold-=1;
            break;
            
            case 356:
            camdest.y+=5;
            break;

        case 357:
            camdest.z=ofClamp(camdest.z+5,-90,90);
            break;
            
        case 358:
            camdest.y-=5;
            break;
            
        case 359:
            camdest.z=ofClamp(camdest.z-5,-90,90);
            break;
        
        case ' ':
#if defined testvid || defined localcamera
            vidplay.setPaused(!vidplay.isPaused());
#endif
            break;
        case 'c':
            drawcam=!drawcam;
            break;   
        case 'f':
            isFPS=!isFPS ;
            break;
        case 'x':
            if( visuHandler.get("particles")){
                Particles * ptmp =(Particles *)visuHandler.get("particles") ;
                ptmp->initFbo();
            };

        default :
            break;
    }
    
    
    
}



//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
#ifndef syphonout
    scrw=width=w;
    scrh=height=h;
    
    visuHandler.scrh = h;
    visuHandler.scrw = w;
    
    finalRender.allocate(width,height,GL_RGB);
#endif
}



void testApp::oscUpdate(){
    ofxOscMessage m;
    while(receiver.getNextMessage(&m)){
 
        
#ifndef PVISU
        if(m.getAddress() == "/polyline"){
            
    #ifdef blobosc  
       // polylastt = ofGetElapsedMillis();
            polyline.clear();
                      
            int n=m.getArgAsInt32(0);
           
            if(n>0){
            for(int k=1;k<n+1;k++){
                ofPoint inp=ofPoint(1-m.getArgAsFloat(2*k-1),1-m.getArgAsFloat(2*k));
                
                if(f.blobxinvert)inp.x=1- inp.x;
                if(f.blobyinvert)inp.y=1- inp.y;
                
                
                polyline.lineTo(inw/2.0+inw*(inp.x-.5)/camrot.x,inh/2+inh*(inp.y-.5)/camrot.x);
            }
            polyline.close();
           // printf("%f\n",ofGetElapsedTimef());
            }
            else{
                polyline. lineTo(-1,-1);
                polyline.lineTo(-1.5,-1.5);
                polyline.close();
                //polyline.clear();
            }
            
           #endif
        }
        

        else
            #endif
            if(m.getAddress() == "/attractors"){
            vector<ofPoint> points;
            vector<int> familly;
            for(int i=0;i<(m.getNumArgs()-1)/3;i++){
                familly.push_back(m.getArgAsInt32(1+i*3));
                
                points.push_back(ofPoint(m.getArgAsFloat(2+i*3),m.getArgAsFloat(3+i*3)));
            }
            attrctl.update(points,familly);
        } 
        else if(m.getAddress() == "/backTrack"){
            backTrack = m.getArgAsInt32(0)>0;
        }
        else if(m.getAddress() == "/zoomback"){
            zoomback = m.getArgAsFloat(0);
            // printf("curzoom %f\n",zoomback);
        }
//        else if(m.getAddress() == "/gravity"){
//            f.gravity=m.getArgAsFloat(0)==1;
//        }
        
        
        
        
        else if(m.getAddress()=="/finalblur"){
            finalblur=m.getArgAsFloat(0);
            
        }
        
        //CAMERA
        
        else if(m.getAddress()=="/camrotx"){
            camdest.y=m.getArgAsFloat(0);
        }
        else if(m.getAddress()=="/camroty"){
            camdest.z=m.getArgAsFloat(0);
        }
        else if(m.getAddress()=="/camrotz"){
            camdest.w=m.getArgAsFloat(0);
        }
        
        else if(m.getAddress()=="/camdist"){
            camdest.x=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/alphacam"){
            alphacamrot=m.getArgAsFloat(0)/100.0;
        }
        
        else if(m.getAddress()=="/brightness"){
            brightness=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/saturation"){
            saturation=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/contrast"){
           contrast=m.getArgAsFloat(0)/100.0;
        }
        
               
        
//        
//        
//        
//        else if (m.getAddress()=="/MyLiveSet/y"){
//            f.yori=scrh/2-m.getArgAsFloat(0)*300.0;
//        }
        
        else if(m.getAddress()=="/drawBlob"){
            drawBlob=m.getArgAsFloat(0)==1;
        }
    
        
        else if(m.getAddress()=="/beat"){
            visuHandler.beat=m.getArgAsFloat(0);
        }
        else if(m.getAddress()=="/visufollowcam"){
            visuHandler.visufollowcam=m.getArgAsFloat(0)==1;
        }

        else if(m.getAddress()=="/audioenv"){
            visuHandler.audioenv=m.getArgAsFloat(0);
        }
        else if(m.getAddress()=="/camwiggledamp"){
            camwiggle.damp=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/camwigglezoomdamp"){
            camwiggle.zoomdamp=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/camwigglezoomamp"){
            camwiggle.zoomamplitude=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/camwiggleamp"){
            camwiggle.amplitude=m.getArgAsFloat(0);
        }
        else if(m.getAddress()=="/camwigglefreq"){
           camwiggle.frequency=m.getArgAsFloat(0)/100.0;
        }
        else if(m.getAddress()=="/camwigglephase"){
            camwiggle.speedPhase=m.getArgAsFloat(0)/100.0;
        }
        
        
        
    }


}
    




void testApp::exit(){
    
    
    
    // videoClient.~ofxSyphonClient();
    //    blobClient.~ofxSyphonClient();
    
}



//--------------------------------------------------------------

#ifndef PVISU
void testApp::readObj(const char * path){
    int maxmesh=numParticles;
   float* oritab= new float[3*maxmesh];
    for(int k = 0 ; k<3*maxmesh;k++){
        oritab[k] = 0;
    }
    
    
    ofFile f(path);
    ofBuffer buf = f.readToBuffer();
    buf.getFirstLine();
    //Boolean initvertex=true;

    
    //ofVec3f minpoint=ofVec3f(999999,9999999,9999999);
    

    
    
    
    int numvertex = 0;
//    while(!buf.isLastLine()&&numvertex<maxmesh){
//        
//        string sbuf=buf.getNextLine();
//        if(sbuf[0]=='v'){
//            
//            //if(initvertex){initvertex=false;}
//            char vbuf[20];
//            int i =2;
//            for (int k=0 ,z=0;k<3;k++,z=0){
//                
//                while((char)sbuf[i]!='\0'&&(char)sbuf[i]!=' '){
//                    vbuf[z]=(char)sbuf[i];
//                    z++;
//                    i++;
//                }
//                vbuf[z]='\0';
//                i++;
//                float mult=0.31;
//                switch (k) {
//                        
//                    case 0:
//                        oritab[numvertex*3]=(float)(ofClamp(atof(vbuf)*mult,0.0,1.0));
//                       // minpoint.x=oritab[numvertex*3]<minpoint.x?oritab[numvertex*3]:minpoint.x;
//                        break;
//                    case 1:
//                        oritab[numvertex*3+1]=(float)(ofClamp(atof(vbuf)*mult,0.0,1.0));
//                       // minpoint.y=oritab[numvertex*3+1]<minpoint.y?oritab[numvertex*3+1]:minpoint.y;
//                        break;
//                    case 2:
//                        oritab[numvertex*3+2]=(float)(ofClamp(atof(vbuf)*mult,0.0,1.0)+0.5);
//                        //minpoint.z=oritab[numvertex*3+2]<minpoint.z?oritab[numvertex*3+2]:minpoint.z;
//                        break;
//                        
//                    default:
//                        break;
//                }
//            } 
//           // if(numvertex%100==0)ofSleepMillis(1);
//             particleStates[numvertex]=true;
//            numvertex++;
//        }
//        
//    }
    
 
    f.close();
    for(int k=numvertex;k<numParticles;k++){
               oritab[3*k]=0.5;
                oritab[3*k+1]=0.5;
                oritab[3*k+2]=0.5;
        
    }
//    for(int k=0;k<numvertex;k++){
//        oritab[3*k]-=minpoint.x;
//        oritab[3*k+1]-=minpoint.y;
//        oritab[3*k+2]-=minpoint.z;
//    }
    
    origins.getTextureReference().loadData(oritab,textureRes,textureRes,GL_RGB);
    
       
       
       
    delete [] oritab;
}








void testApp::reset(){
    posPingPong.src= &origins;
    
    
    float * velinit = new float[numParticles*3];
    for (int i = 0; i < numParticles; i++){
        velinit[i*3 + 0] = 0.;//+ofRandom(-.1,.1);
        velinit[i*3 + 1] = 0.;//+ofRandom(-.1,.1);
        velinit[i*3 + 2] = 0.;
    }
    velPingPong.src->getTextureReference().loadData(velinit, textureRes, textureRes, GL_RGB);
    
    delete [] velinit;
}








 



void testApp::changeOrigin(int type){
    Float32* oritab= new Float32[3*numParticles];
    
    switch (type) {
            
        case 0:
        {
            for (int x = 0; x < textureRes; x++){
                for (int y = 0; y < textureRes; y++){
                    int i = textureRes * y + x;
                    oritab[i*3 + 0] = x*1.0/textureRes;
                    oritab[i*3 + 1] = y*1.0/textureRes;
                    oritab[i*3 + 2] = 0.5;
                }
            }
               origins.getTextureReference().loadData(oritab,textureRes,textureRes,GL_RGB);
            
            
        }
               
            break;
            
        case 1:
        {
            int numcircles=textureRes;
            int numangl=textureRes;
            float ray=0.4;
            for(int k=0;k<numcircles;k++){
                float y=0.5-ray+2.0*ray*k/numcircles;
                
                for(int j=0;j<numangl;j++){
                    float x =  0.5+sqrtf(ray*ray-1.0*(y-0.5)*(y-0.5))*cos(j*1.0/numangl*2.0*PI);
                    float z = 0.5+sqrtf(ray*ray-1.0*(y-0.5)*(y-0.5))*sin(j*1.0/numangl*2.0*PI);
                    
                    oritab[3*(k*numangl+j)+0]=x;
                    oritab[3*(k*numangl+j)+1]=y;
                    oritab[3*(k*numangl+j)+2]=z;
                    
 
                    
                }
                             }
            origins.getTextureReference().loadData(oritab,textureRes,textureRes,GL_RGB);


            
           
        }   
             break;
        case 2:
        {
 

            for (int x = 0; x < textureRes; x++){
                for (int y = 0; y < textureRes; y++){
                    int i = textureRes * y + x;
                    
                    oritab[i*3 + 0] = 0;
                    oritab[i*3 + 1] = y*1.0/textureRes;
                    oritab[i*3 + 2] = 0.5;
                }
            }
            

            origins.getTextureReference().loadData(oritab, textureRes, textureRes, GL_RGB);
            
          
        }
            break;
        case 3:
        {
//            ofxAssimpModelLoader model;
//            model.loadModel("obj/text-100170.obj",false);
//            ofMesh mesh  = model.getMesh(0);
////            mesh.getBoundingBox()
//               model.clear();
//            float scale = 0.1;
//            ofVec3f pos = ofVec3f(0.5,0,0.5);
//            ofVec3f min, max;
//            min = ofVec3f(99999,99999,99999);
//            max = ofVec3f(-9999,-9999,-9999);
//            
//            for (int i = 0 ; i< mesh.getNumVertices();i++){
//                min.x = mesh.getVertex(i).x<min.x?mesh.getVertex(i).x:min.x;
//                min.y = mesh.getVertex(i).y<min.y?mesh.getVertex(i).y:min.y;
//                min.z = mesh.getVertex(i).z<min.z?mesh.getVertex(i).z:min.z;
//                
//                max.x = mesh.getVertex(i).x>max.x?mesh.getVertex(i).x:max.x;
//                max.y = mesh.getVertex(i).y>max.y?mesh.getVertex(i).y:max.y;
//                max.z = mesh.getVertex(i).z>max.z?mesh.getVertex(i).z:max.z;
//            }
//            
//            
//            pos = -(min+max)/2.;
//            scale = 1./(max.x-min.x);
//            for (int i = 0 ; i<(textureRes-1)*(textureRes-1);i++){
//
//                if(i< mesh.getNumVertices()){
//                    oritab[i*3 + 0] = (Float32)CLAMP(0.5+(mesh.getVertex(i).x+ pos.x)*scale ,0,1);
//                    oritab[i*3 + 1] = (Float32)CLAMP(0.5+(mesh.getVertex(i).y+ pos.y)*scale ,0,1);
//                    oritab[i*3 + 2] = (Float32)CLAMP(0.5+(mesh.getVertex(i).z+ pos.z)*scale ,0,1);
//
//                    
//                }
//                else{
//                    
//                    oritab[i*3 + 0] = 0.5;
//                    oritab[i*3 + 1] = 0.5;
//                    oritab[i*3 + 2] = 0.5;
//                }
//                
//                                    
//            }
//            mesh.clear();
//         
//            
//            
//            origins.getTextureReference().loadData(oritab, textureRes, textureRes, GL_RGB);
//
//            
//            
//            
        }
            break;
        case 4:
        {
            
            
            for (int x = 0; x < textureRes; x++){
                for (int y = 0; y < textureRes; y++){
                    int i = textureRes * y + x;
                    
                    oritab[i*3 + 0] = x*1.0/textureRes;
                    oritab[i*3 + 1] = y*1.0/textureRes;
                    oritab[i*3 + 2] = sqrt((x*1.0/textureRes-0.5)*(x*1.0/textureRes-0.5))+sqrt((y*1.0/textureRes-0.5)*(y*1.0/textureRes-0.5));

                }
            }
            
            
            origins.getTextureReference().loadData(oritab, textureRes, textureRes, GL_RGB);
            
            
        }
            break;
        default:
            break;
            
            
            
    }
       delete [] oritab; 
    
 
}







ofVec3f testApp::camToWorld(ofVec3f vecin){
    float ph=ofDegToRad(camrot.y);
    float th=ofDegToRad(camrot.z);
    
    ofMatrix3x3 rotmat=ofMatrix3x3(cos(ph),      0,         sin(ph),
                                   sin(ph)*sin(th), cos(th), -cos(ph)*sin(th),
                                   -cos(th)*sin(ph), sin(th),  cos(ph)*cos(th));
    rotmat.invert();
    
    return ofVec3f(vecin.dot(ofVec3f(rotmat[0],rotmat[1],rotmat[2])),vecin.dot(ofVec3f(rotmat[3],rotmat[4],rotmat[5])),vecin.dot(ofVec3f(rotmat[6],rotmat[7],rotmat[8])));
}


void testApp::oscBlobUpdate(){
   // blobHandler.update();
#ifdef blobosc 
    
    
    vector<ofPolyline> & polys = polyline.getOutline();
    for(int i=0;i<(int)polys.size();i++)
        if(polys[0].size()>0){
            ofPolyline cp=polys[0];
            if(f.blobmirrorx){
                for(int k = 0;k<cp.size();k++){
                    polyline.lineTo(inw-cp[k].x,cp[k].y) ;
                }
                polyline.close();
            }
            if(f.blobmirrory){
                for(int k = 0;k<cp.size();k++){
                    polyline.lineTo(cp[k].x,inh-cp[k].y) ;
                }
                polyline.close();   
            }
        }
    
    
    
    
    ofPushStyle();

    polyBlob.src->begin();
    ofSetColor(ofColor(0,0,0,ablob));
    ofFill();
    ofRect(0,0,inw,inh);

    ofSetColor(255);
    // ofNoFill();
    
    //printf("s %d",(polyline.getOutline()).size());
    polyline.setFillColor(ofColor(255));
    polyline.draw();
    visuHandler.drawFbo();
    
    polyBlob.src->end();
   // if( polyline.getOutline().size()>0){
        
            if(f.blobblur>0){
        polyBlob.dst->begin();
        Shadow.begin();
        Shadow.setUniformTexture("texture",polyBlob.src->getTextureReference(),0);
        Shadow.setUniform1f("offset",f.blobblur);
        Shadow.setUniform1f("direction_x",1.0);
            Shadow.setUniform1f("brightness",brightness2);
            Shadow.setUniform1f("saturation",saturation2);
            Shadow.setUniform1f("contrast",contrast2);

        ///Just a frame to put pixels on
        ofSetColor(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(inw, 0); glVertex3f(inw, 0, 0);
        glTexCoord2f(inw,inh); glVertex3f( inw,inh, 0);
        glTexCoord2f(0, inh);  glVertex3f(0, inh, 0);
        glEnd();
        
        
           Shadow.end();
        polyBlob.dst->end();
        polyBlob.swap();
        
        polyBlob.dst->begin();
        Shadow.begin();
        Shadow.setUniformTexture("texture",polyBlob.src->getTextureReference(),0);
        Shadow.setUniform1f("offset",f.blobblur);
        Shadow.setUniform1f("direction_x",0.0);
            Shadow.setUniform1f("contrast",contrast2);
            Shadow.setUniform1f("saturation",saturation2);    
            Shadow.setUniform1f("brightness",brightness2);
        ///Just a frame to put pixels on
        ofSetColor(255,255,255,255);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(inw, 0); glVertex3f(inw, 0, 0);
        glTexCoord2f(inw,inh); glVertex3f( inw,inh, 0);
        glTexCoord2f(0, inh);  glVertex3f(0, inh, 0);
        glEnd();
        
        
        Shadow.end();
        polyBlob.dst->end();
        polyBlob.swap();
            }

    
   // }

    
  
    ofPopStyle();
   
#endif 
    
    
}

#endif

#ifdef MOUSEATTR
void testApp::mouseDragged(int x, int y, int button){
    vector<ofPoint> points;
    vector<int> familly;
  
        familly.push_back(0);
        points.push_back(ofPoint(1.0-x*1.0/scrw,y*1.0/scrh,0));

    if(points.size()>0)attrctl.update(points,familly);
//    printf("%i",button);
}
void testApp::mouseReleased(int x, int y, int button){
    vector<ofPoint> points;
    vector<int> familly;
    familly.clear();
    points.clear();
    
    attrctl.update(points,familly);
    
}
#endif
#endif

