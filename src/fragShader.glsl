#version 150

in vec3 oColor;
out vec4 colour; 

void main()
{

		colour = vec4(oColor, 1.0); 
	
}

