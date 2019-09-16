#version 330
 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 frag_pos;
out vec3 frag_norm;
out vec2 frag_uv;

out vec4 light_pos;
out vec4 view_pos;

void main(void)
{
    
    light_pos = view * vec4(0.0,10.0,0.0,1.0);
	view_pos = vec4(view[0].w,
					view[1].w,
					view[2].w,
					view[3].w  
					);


    frag_uv = uv;
    frag_norm = normalize( vec3(model * vec4(normal, 0.0)) );
	frag_pos = view * model * vec4(position, 1.0);
    gl_Position = projection * frag_pos ;
}
