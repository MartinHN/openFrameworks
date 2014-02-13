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
    vec3 poso = texture2DRect( posData , st+vec2(0,1)).xyz;
    vec3 opos = texture2DRect( posData , st+vec2(0,-1)).xyz;    
    
    
    float l0norm = l0/resolution;
    vec3 distbuf;
    

    float netmax = 100*l0norm;
    float normbuf;
    vec3 linbuf;

        
    if(st.y<resolution-1){
            distbuf=pos-poso;
            linbuf=distbuf;
            normbuf = length(distbuf);
            if(normbuf<netmax){
            vel-=normalize(distbuf)*(normbuf-l0norm)*k;
            }
        }
    if(st.y>1){
        distbuf=pos-opos;
        normbuf = length(distbuf);
        
        if(normbuf<netmax){
            vel-=normalize(distbuf)*(normbuf-l0norm)*k;
            if(st.y<resolution-1&&length(distbuf)<netmax){
                vel-=z*(distbuf+linbuf);
            }
        }
        

        
    }
    
    


    
    
    

    
    gl_FragColor = vec4(vel.x,vel.y,vel.z,1.0);

}