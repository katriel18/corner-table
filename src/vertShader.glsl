#version 330

in vec2 iPosition;

 in vec3 iColor;

out vec3 oColor;

void main()
{
 	oColor = iColor;
 	//oColor = vec3(0.0,1.0,0.0);
 	
   gl_Position = vec4(iPosition, 0.0, 1.0);
}