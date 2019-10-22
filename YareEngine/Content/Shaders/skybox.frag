#version 330

in vec3 uvw;
out vec4 out_color;

uniform samplerCube environment;

void main()
{    
    out_color = texture(environment, uvw);
	
}