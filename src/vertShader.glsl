#version 150

in vec2 iPosition;

void main()
{
 
   gl_Position = vec4(0.5*iPosition, 0.0, 1.0);
}