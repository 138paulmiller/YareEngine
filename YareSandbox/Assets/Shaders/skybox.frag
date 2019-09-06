#version 330

in vec4 frag_position;
out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, frag_position.xyz);
	color = vec4(1,0, 0,1);
}