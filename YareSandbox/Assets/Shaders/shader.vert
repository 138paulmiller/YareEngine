#version 330
 in vec3 position;
 in vec3 normal;
 in vec2 uv;

uniform mat4 projection;
uniform mat4 view;

out vec4 frag_position;
out vec3 frag_normal;
out vec2 frag_uv;

out vec4 light_pos;

void main(void)
{
    
    light_pos = view * vec4(0.0,0.0,1.0,1.0);

    frag_normal = vec3(view * vec4(normal,0.0));
    frag_uv = uv;

	frag_position = view * vec4(position,1.0);
    gl_Position = projection * frag_position;
}
