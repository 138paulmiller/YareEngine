#version 330
in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec4 frag_position;
out vec4 light_pos;

void main(void)
{    
	frag_position = view * vec4(position,1.0);
    gl_Position = projection * frag_position;
}
