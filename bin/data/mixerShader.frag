#version 120

uniform float t_input01 = 1.0;
uniform float t_input02 = 1.0;

uniform sampler2DRect input01;
uniform sampler2DRect input02;

void main(void)
{
    vec2 texCoord = gl_TexCoord[0].xy;
    
    vec4 p_input01 = texture2DRect(input01, texCoord);
    vec4 p_input02 = texture2DRect(input02, texCoord);
    
    gl_FragColor   =   t_input01 * p_input01   +   t_input02 * p_input02;
    //gl_FragColor = p_input01+p_input02;
    //gl_FragColor = vec4(texCoord.x/1280.0f,texCoord.y/720.0f,0,1);
    
}