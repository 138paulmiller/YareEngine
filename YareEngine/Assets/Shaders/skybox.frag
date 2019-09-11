#version 330

in vec3 uvw;
out vec4 color;

uniform samplerCube environment;

void main()
{    
    color = texture(environment, uvw);
}