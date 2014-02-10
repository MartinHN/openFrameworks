#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
#define GL_FRAGMENT_PRECISION_HIGH 1





uniform sampler2DRect tex0;   // recive the previus velocity texture
uniform sampler2DRect posData;      // recive the position texture

uniform int resolution;
uniform vec2  screen;



// forces

uniform float k;
uniform float l0;
uniform float z;



    
void main(void){
    vec2 st=gl_TexCoord[0].xy;
    vec3 vel = texture2DRect(tex0, st).xyz;
    vec3 pos = texture2DRect( posData, st).xyz;
    vec3 poso = texture2DRect( posData , st+vec2(1,0)).xyz;
    vec3 opos = texture2DRect( posData , st+vec2(-1,0)).xyz;    
    
    
    
    vec3 distbuf;
    

    float netmax = 50*l0;
    float normbuf;
    vec3 linbuf;
    linbuf.x=netmax+1;
        
    if(st.x<resolution-1){
            distbuf=pos-poso;
            linbuf=distbuf;
            normbuf = length(distbuf);
            if(normbuf<netmax){
            vel+=normalize(distbuf)*(normbuf-l0)*k;            
            }
        }
    if(st.x>1){
        distbuf=pos-opos;
        normbuf = length(distbuf);
        
        if(normbuf<netmax){
            vel+=normalize(distbuf)*(normbuf-l0)*k;            
        }
        
        if(length(linbuf)<netmax){
            vel+=z*(distbuf+linbuf);
        }
        
    }
    
    


    
    
    

    
    gl_FragColor = vec4(vel.x,vel.y,vel.z,1.0);

}