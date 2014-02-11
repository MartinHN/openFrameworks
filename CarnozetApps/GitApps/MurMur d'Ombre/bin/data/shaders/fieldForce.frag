#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
#define GL_FRAGMENT_PRECISION_HIGH 1





uniform sampler2DRect tex0;   // recive the previus velocity texture
uniform sampler2DRect posData;      // recive the position texture
uniform sampler2DRect fieldData;      // recive the origin

uniform int resolution;
uniform vec2  screen;


uniform vec2 inres;
// forces

uniform float k;
uniform float veloutz;
uniform float velouty;
uniform float veloutborder;
uniform float minv;
uniform float maxv;




    
void main(void){

    
    vec2 st =gl_TexCoord[0].xy;
    vec3 vel = texture2DRect(tex0, st).xyz;
    vec2 pos = texture2DRect( posData, st).xy * inres;
    
    float field =  texture2DRect( fieldData, pos).x;

    
    
    if (field>maxv&&velouty*veloutz>0){
        vel+=vec3(0,velouty,veloutz);
        
    }
    else if(field>minv){
        vel+= vec3 (k*(field-texture2DRect( fieldData , pos+vec2(1,0)).x) , k*(field-texture2DRect( fieldData , pos+vec2(0,1)).x) , veloutborder);
    }

    
    gl_FragColor = vec4(vel.x,vel.y,vel.z,1.0);

}