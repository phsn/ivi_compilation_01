#version 120

#define TWO_PI 6.28318530718

uniform int copies = 3;
uniform float rotation = 20;
uniform float scale = 0.5;

uniform vec2 screenSize = vec2( 1280.0, 720.0 );
uniform sampler2DRect inputTexture;

void main(void)
{
    vec2 texCoord = gl_TexCoord[0].xy;
    vec2 center = vec2(screenSize.x/2.0f,screenSize.y/2.0f);
    vec2 cV = 1.1*scale*(texCoord-center);
    
    vec4 outputP = vec4(0,0,0,1.0);
    
    float degrees = rotation*TWO_PI/360;
    
    for(int i=0; i < int(copies/2.0f); i++) {
        outputP += texture2DRect( inputTexture, center+cV);
        cV *= scale;
        cV = vec2(cV.x * cos(degrees) - cV.y * sin(degrees),
                  cV.x * sin(degrees) + cV.y * cos(degrees));
    }
    
    cV = (texCoord-center);
    
    for(int i=0; i < copies; i++) {
        outputP += (1-i/float(copies))*texture2DRect( inputTexture, center+cV);
        cV *= 1+scale;
        cV = vec2(cV.x * cos(-degrees) - cV.y * sin(-degrees),
                  cV.x * sin(-degrees) + cV.y * cos(-degrees));
    }
    
    gl_FragColor = outputP;
    
}