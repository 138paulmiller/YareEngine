#version 330
in vec3 position;
in vec3 normal;
in vec2 uv;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 uvw;

void main(void)
{    
	uvw = position.xyz ;
    vec4 frag_position = projection * view * model * vec4(position,1.0);
	gl_Position = frag_position.xyww;
}
