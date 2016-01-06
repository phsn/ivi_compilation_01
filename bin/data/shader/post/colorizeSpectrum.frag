#version 120

uniform float spectrumPosition = 0.0;
uniform vec2 screenSize = vec2( 1280.0, 720.0 );
uniform sampler2DRect inputTexture;
uniform sampler2DRect spectrum;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


void main(void)
{
    vec2 texCoord = gl_TexCoord[0].xy;
    
    vec4 inputColor = texture2DRect(inputTexture, texCoord);
    float brightness = (inputColor.r*0.299f) + (inputColor.g*0.587f) + (inputColor.b*0.114f);
    
    //vec3 inputHsv = rgb2hsv(inputColor.xyz);
    vec4 outputColor = texture2DRect(spectrum,vec2(spectrumPosition*500,(1.0f-brightness)*255));
    
    //vec3 outputHsv = rgb2hsv(outputColor.xyz);
    
    //vec3 resultHsv = vec3(outputHsv.x,outputHsv.y,outputHsv.z);
    //vec3 resultRgb = hsv2rgb(resultHsv);
    
    //gl_FragColor = vec4(resultRgb,1.0);
    gl_FragColor = outputColor;
    
}