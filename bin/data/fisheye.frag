#version 120
#extension GL_ARB_texture_rectangle: enable

uniform vec2 iResolution;
uniform vec2 iMouse;
uniform float chromAbb;

uniform sampler2DRect inputTexture;

#define EPSILON (0.000011)

void main(void)//Drag mouse over rendering area
{
    
    float chromDiff = chromAbb/10000.0f;
    //normalized coords with some cheat
    vec2 p = gl_TexCoord[0].xy / iResolution.x;
    
    //screen proportion
    float prop = iResolution.x / iResolution.y;
    //center coords
    vec2 m = vec2(0.5, 0.5 / prop-0.2);
    //vector from center to current fragment
    vec2 d = p - m;
    // distance of pixel from center
    float r = sqrt(dot(d, d));
    
    //amount of effect
    float power = ( 2.0 * 3.141592 / (2.0 * sqrt(dot(m, m))) ) *
    (iMouse.x / iResolution.x - 0.5);
    //radius of 1:1 effect
    float bind;
    if (power > 0.0) bind = sqrt(dot(m, m));//stick to corners
    else {if (prop < 1.0) bind = m.x; else bind = m.y;}//stick to borders
    
    //Weird formulas
    vec2 uv;
    if (power > 0.0)//fisheye
        uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
    else if (power < 0.0)//antifisheye
        uv = m + normalize(d) * atan(r * -power * 10.0) * bind / atan(-power * bind * 10.0);
    else
        uv = p;//no effect for power = 1.0
    
    //Second part of cheat
    //for round effect, not elliptical
    vec4 colorA = texture2DRect(inputTexture, vec2(uv.x, uv.y * prop)*iResolution.x);
    
     power = ( 2.0 * 3.141592 / (2.0 * sqrt(dot(m, m))) ) *
    (iMouse.x / iResolution.x - 0.5)*(1-chromDiff);
    //radius of 1:1 effect
     bind;
    if (power > 0.0) bind = sqrt(dot(m, m));//stick to corners
    else {if (prop < 1.0) bind = m.x; else bind = m.y;}//stick to borders
    
    //Weird formulas
    if (power > 0.0)//fisheye
        uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
    else if (power < 0.0)//antifisheye
        uv = m + normalize(d) * atan(r * -power * 10.0) * bind / atan(-power * bind * 10.0);
    else
        uv = p;//no effect for power = 1.0
    
    //Second part of cheat
    //for round effect, not elliptical
    vec4 colorB = texture2DRect(inputTexture, vec2(uv.x, uv.y * prop)*iResolution.x);
    
    power = ( 2.0 * 3.141592 / (2.0 * sqrt(dot(m, m))) ) *
    (iMouse.x / iResolution.x - 0.5)*(1+chromDiff);
    //radius of 1:1 effect
    bind;
    if (power > 0.0) bind = sqrt(dot(m, m));//stick to corners
    else {if (prop < 1.0) bind = m.x; else bind = m.y;}//stick to borders
    
    //Weird formulas
    if (power > 0.0)//fisheye
        uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
    else if (power < 0.0)//antifisheye
        uv = m + normalize(d) * atan(r * -power * 10.0) * bind / atan(-power * bind * 10.0);
    else
        uv = p;//no effect for power = 1.0
    
    //Second part of cheat
    //for round effect, not elliptical
    vec4 colorC = texture2DRect(inputTexture, vec2(uv.x, uv.y * prop)*iResolution.x);

    
    gl_FragColor = vec4(colorC.x,colorA.y,colorB.z, 1.0);
    //gl_FragColor = vec4(uv.x,uv.y,0.0,1.0);
    
}