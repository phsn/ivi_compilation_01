

#version 120
#extension GL_ARB_texture_rectangle: enable

#define PI (3.14159265358979323846)
#define TWO_PI (2*PI)
#define SQRT_3 (1.732050808)

uniform int numRepetitions = 10;
uniform vec2 screenSize = vec2( 1280.0, 720.0 );
uniform sampler2DRect inputTexture;

void main(){
    vec2 texCoord = gl_TexCoord[0].xy;
    
    vec2 triangleSize = vec2(2*screenSize.y/SQRT_3,screenSize.y);
    vec2 bucketSize = vec2((screenSize.y/numRepetitions)/SQRT_3,screenSize.y/numRepetitions);
    vec2 bucketIdx = vec2(floor(texCoord.x/bucketSize.x),floor((screenSize.y-texCoord.y)/bucketSize.y));
    vec2 bucketCoord = vec2(mod(texCoord.x,bucketSize.x),mod(texCoord.y,bucketSize.y));
    vec2 samplePos;
    
    float bucketClass = mod(bucketIdx.x,6);

    if(mod(bucketIdx.y,2) == 0) {
        bucketCoord = vec2(bucketCoord.x,bucketSize.y-bucketCoord.y);
    }
    
    if (mod(bucketClass,2) == 0) {
        if((bucketSize.x-bucketCoord.x)*screenSize.x/screenSize.y > bucketCoord.y) {
            // OBEN LINKS
            samplePos = vec2(screenSize.x/2.0-(bucketCoord.x/bucketSize.x)*triangleSize.x/2,
                             triangleSize.y-bucketCoord.y/bucketSize.y*triangleSize.y);
            
            if(bucketClass == 0) {
                vec2 origin = vec2(bucketSize.x,bucketSize.y);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a -= PI/3.0;
            
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(screenSize.x*0.982,screenSize.y/2.0*1.01);
                samplePos = vec2(screenSize.x-u.x,u.y);
            } else if(bucketClass == 2) {
                vec2 origin = vec2(-bucketSize.x,bucketSize.y);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(25,screenSize.y/2.0);
                samplePos = vec2(screenSize.x-u.x,u.y);
            } else if(bucketClass == 4) {
                vec2 origin = vec2(0,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += PI;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(screenSize.x/2.0,screenSize.y);
                samplePos = vec2(screenSize.x-u.x,u.y);
            }
            
        } else {
            // UNTEN LINKS
            samplePos = vec2((bucketCoord.x/bucketSize.x)*triangleSize.x/2+(screenSize.x-triangleSize.x)/2.0,
                             bucketCoord.y/bucketSize.y*triangleSize.y);
            if(bucketClass == 2) {
                vec2 origin = vec2(bucketSize.x*2.0,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a -= 2*PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(8,screenSize.y/2.0*1.02);
                samplePos = vec2(u.x,u.y);
            } else if(bucketClass == 4) {
                vec2 origin = vec2(0,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += 2*PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y))*0.99;
                u += vec2(screenSize.x*0.99,screenSize.y/2.0);
                samplePos = vec2(u.x,u.y);
            }
            //samplePos = vec2(screenSize.x/2.0,screenSize.y*1.8);
        }
    } else {
        if(bucketCoord.x*screenSize.x/screenSize.y > bucketCoord.y) {
            // OBEN RECHTS
            samplePos = vec2((screenSize.x-triangleSize.x)/2.0-(bucketCoord.x/bucketSize.x)*triangleSize.x/2.0+triangleSize.x,
                              triangleSize.y-bucketCoord.y/bucketSize.y*triangleSize.y);
            if(bucketClass == 1) {
                vec2 origin = vec2(-0.9,bucketSize.y);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(25,screenSize.y/2.0);
                samplePos = vec2(screenSize.x-u.x,u.y);
            } else if(bucketClass == 3) {
                vec2 origin = vec2(bucketSize.x,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += PI;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(screenSize.x/2.0,screenSize.y);
                samplePos = vec2(screenSize.x-u.x,u.y);
            } else if(bucketClass == 5) {
                vec2 origin = vec2(bucketSize.x*2.0,bucketSize.y);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a -= PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y))*1.002;
                u += vec2(screenSize.x*0.982,screenSize.y/2.0*1.01);
                samplePos = vec2(screenSize.x-u.x,u.y);
            }

        } else {
            // UNTEN RECHTS
            samplePos = vec2((bucketCoord.x/bucketSize.x)*triangleSize.x/2+screenSize.x/2.0,
                             bucketCoord.y/bucketSize.y*triangleSize.y);
            if(bucketClass == 3) {
                vec2 origin = vec2(bucketSize.x,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a -= 2*PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(8,screenSize.y/2.0*1.02);
                samplePos = vec2(u.x,u.y);
            } else if(bucketClass == 5) {
                vec2 origin = vec2(-bucketSize.x,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += 2*PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y))*0.99;
                u += vec2(screenSize.x*0.99,screenSize.y/2.0);
                samplePos = vec2(u.x,u.y);
            }
        }
    }
    
    vec4 ColorA = texture2DRect( inputTexture, samplePos );
    
     bucketSize = vec2((screenSize.y/(numRepetitions*1.5))/SQRT_3,screenSize.y/(numRepetitions*1.5));
     bucketIdx = vec2(floor(texCoord.x/bucketSize.x),floor((screenSize.y-texCoord.y)/bucketSize.y));
     bucketCoord = vec2(mod(texCoord.x,bucketSize.x),mod(texCoord.y,bucketSize.y));
    
     bucketClass = mod(bucketIdx.x,6);
    
    if(mod(bucketIdx.y,2) == 0) {
        bucketCoord = vec2(bucketCoord.x,bucketSize.y-bucketCoord.y);
    }
    
    if (mod(bucketClass,2) == 0) {
        if((bucketSize.x-bucketCoord.x)*screenSize.x/screenSize.y > bucketCoord.y) {
            // OBEN LINKS
            samplePos = vec2(screenSize.x/2.0-(bucketCoord.x/bucketSize.x)*triangleSize.x/2,
                             triangleSize.y-bucketCoord.y/bucketSize.y*triangleSize.y);
            
            if(bucketClass == 0) {
                vec2 origin = vec2(bucketSize.x,bucketSize.y);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a -= PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(screenSize.x*0.982,screenSize.y/2.0*1.01);
                samplePos = vec2(screenSize.x-u.x,u.y);
            } else if(bucketClass == 2) {
                vec2 origin = vec2(-bucketSize.x,bucketSize.y);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(25,screenSize.y/2.0);
                samplePos = vec2(screenSize.x-u.x,u.y);
            } else if(bucketClass == 4) {
                vec2 origin = vec2(0,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += PI;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(screenSize.x/2.0,screenSize.y);
                samplePos = vec2(screenSize.x-u.x,u.y);
            }
            
        } else {
            // UNTEN LINKS
            samplePos = vec2((bucketCoord.x/bucketSize.x)*triangleSize.x/2+(screenSize.x-triangleSize.x)/2.0,
                             bucketCoord.y/bucketSize.y*triangleSize.y);
            if(bucketClass == 2) {
                vec2 origin = vec2(bucketSize.x*2.0,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a -= 2*PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(8,screenSize.y/2.0*1.02);
                samplePos = vec2(u.x,u.y);
            } else if(bucketClass == 4) {
                vec2 origin = vec2(0,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += 2*PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y))*0.99;
                u += vec2(screenSize.x*0.99,screenSize.y/2.0);
                samplePos = vec2(u.x,u.y);
            }
            //samplePos = vec2(screenSize.x/2.0,screenSize.y*1.8);
        }
    } else {
        if(bucketCoord.x*screenSize.x/screenSize.y > bucketCoord.y) {
            // OBEN RECHTS
            samplePos = vec2((screenSize.x-triangleSize.x)/2.0-(bucketCoord.x/bucketSize.x)*triangleSize.x/2.0+triangleSize.x,
                             triangleSize.y-bucketCoord.y/bucketSize.y*triangleSize.y);
            if(bucketClass == 1) {
                vec2 origin = vec2(-0.9,bucketSize.y);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(25,screenSize.y/2.0);
                samplePos = vec2(screenSize.x-u.x,u.y);
            } else if(bucketClass == 3) {
                vec2 origin = vec2(bucketSize.x,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += PI;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(screenSize.x/2.0,screenSize.y);
                samplePos = vec2(screenSize.x-u.x,u.y);
            } else if(bucketClass == 5) {
                vec2 origin = vec2(bucketSize.x*2.0,bucketSize.y);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a -= PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y))*1.002;
                u += vec2(screenSize.x*0.982,screenSize.y/2.0*1.01);
                samplePos = vec2(screenSize.x-u.x,u.y);
            }
            
        } else {
            // UNTEN RECHTS
            samplePos = vec2((bucketCoord.x/bucketSize.x)*triangleSize.x/2+screenSize.x/2.0,
                             bucketCoord.y/bucketSize.y*triangleSize.y);
            if(bucketClass == 3) {
                vec2 origin = vec2(bucketSize.x,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a -= 2*PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y));
                u += vec2(8,screenSize.y/2.0*1.02);
                samplePos = vec2(u.x,u.y);
            } else if(bucketClass == 5) {
                vec2 origin = vec2(-bucketSize.x,0);
                vec2 v = bucketCoord-origin;
                float r = length(v);
                float a = atan( v.y, v.x );
                a += 2*PI/3.0;
                
                vec2 u = vec2( cos(a), sin(a) ) * r * (screenSize.y/(bucketSize.y))*0.99;
                u += vec2(screenSize.x*0.99,screenSize.y/2.0);
                samplePos = vec2(u.x,u.y);
            }
        }
    }
    
    vec4 ColorB = texture2DRect( inputTexture, samplePos );
    
    gl_FragColor = vec4(1.0,1.0,1.0,3.0)-ColorA*(ColorB+(1*0.9));
 
    
}