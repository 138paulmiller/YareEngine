#version 330

//Quad Format
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
out vec2 frag_uv;

void main(void)
{
	frag_uv = uv;
	gl_Position = vec4(position.xyz, 1.0);
}
