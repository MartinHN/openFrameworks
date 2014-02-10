#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
#define GL_FRAGMENT_PRECISION_HIGH 1





uniform sampler2DRect tex0;   // recive the previus velocity texture
uniform sampler2DRect posData;      // recive the position texture
uniform sampler2DRect originData;      // recive the origin

uniform int resolution;
uniform vec2  screen;



// forces

uniform float k;
uniform float z;
uniform float freeze;



    
void main(void){
//    float mult=screen.x;
//    float screenratio=screen.y/screen.x;
//    vec2 st = gl_TexCoord[0].xy;    // gets the position of the pixel that it´s dealing with...
//    
//    
// 
//    vec3 pos = texture2DRect( posData, st).xyz;
//    
//   
//    
//    pos.y*=screenratio;
//    pos*=mult;
//    // ... for gettinh the position data 
//    vec3 vel = texture2DRect( tex0, st ).xyz;
//    vel.y*=screenratio;
//    vel*=mult;
//    
//    //origin pos
//    vec3 ori =  texture2DRect( originData, st).xyz;
//    ori.y*=screenratio;
//    ori*=mult;
//    
//    //distance
//    ori-=pos;  
//    
//    
//    vec3 prevvel = vel;
//    vec3 velbuf;
//
////    if(freeze>0&&length(ori)<freeze){
////                vel*=0.5;
////
////        }
//    
//    velbuf = ori*k;
//    vel+=velbuf-normalize(velbuf)*dot(prevvel,normalize(velbuf))*z;
//  

//    
//    
//    //vel+=ori*origin.y*(1+origin.z)-origin.z*prevvel;        
//    

    vec3 vel = texture2DRect(tex0, gl_TexCoord[0].xy).xyz;
    
    vec3 ori =  texture2DRect( originData, gl_TexCoord[0].xy).xyz;
    vec3 pos = texture2DRect( posData, gl_TexCoord[0].xy).xyz;
    
    
    vec3 velbuf = (ori-pos)*k;
    
    vel+=velbuf-normalize(velbuf)*dot(vel,normalize(velbuf))*z;
    
    if(freeze>0&&length(ori-pos)<freeze){
        vel*=0.5;
    }
//     vel.x+= 0.1;
//    if(vel.x>1.0){
//     vel.x=0;   
//    }
  
    

    
    gl_FragColor = vec4(vel.x,vel.y,vel.z,1.0);

}