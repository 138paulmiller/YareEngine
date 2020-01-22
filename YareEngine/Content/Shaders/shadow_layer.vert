#version 330

//Quad Format
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
out vec2 frag_uv;
out vec4 light_space_pos;

uniform sampler2D position;

void main(void)
{
	frag_uv = uv;
	vec4 position = texture(position, frag_uv);
	light_space_pos = light_space * vec4(position.xyz,1.0); 

    gl_Position = vec4(position.xyz , 1.0) ;
}
