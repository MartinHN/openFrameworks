#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
#define GL_FRAGMENT_PRECISION_HIGH 1





uniform sampler2DRect tex0;   // recive the previus velocity texture
uniform sampler2DRect posData;      // recive the position texture

uniform int resolution;
uniform vec2  screen;



// forces
uniform vec3 r;
uniform float damp;
uniform vec3 vmin;



    
void main(void){
    
    vec3 vel = texture2DRect(tex0, gl_TexCoord[0].xy).xyz;
    vec3 pos = texture2DRect( posData, gl_TexCoord[0].xy).xyz;
    
    
    if(pos.x>1.0-r.x ){
        vel.x=-abs(vel.x)*damp-vmin.x;
    }
    if(pos.x<r.x ){
        vel.x=abs(vel.x)*damp+vmin.x;
    }
    if(pos.y>1.0-r.y ){
        vel.y=-abs(vel.y)*damp-vmin.y;
    }
    if(pos.y<r.y ){
        vel.y=abs(vel.y)*damp+vmin.y;
    }
    if(pos.z>1.0-r.z ){
        vel.z=-abs(vel.z)*damp-vmin.z;
    }
    if(pos.z<r.z ){
        vel.z=+abs(vel.z)*damp+vmin.z;
    }
    
    gl_FragColor.xyz = vel;

}