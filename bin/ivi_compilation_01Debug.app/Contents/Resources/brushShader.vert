#version 120

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
    
    gl_PointSize = gl_Normal.y*gl_Normal.x;
    
    if(floor(gl_Normal.y*gl_Normal.x) == 0) {
        gl_FrontColor = vec4(0,0,0,0);
    }
}