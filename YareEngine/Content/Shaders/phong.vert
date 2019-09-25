#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 frag_pos;
out vec3 frag_normal;
out vec2 frag_uv;

void main(void)
{

    frag_uv = uv;
    frag_normal = vec3(model * vec4(normal,0));
	frag_pos =  model * vec4(position, 1.0);
    gl_Position = projection * view * frag_pos ;
}
