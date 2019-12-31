#version 330

in vec4 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

in vec4 view_pos;

//Create a switch for using either map or float3
uniform struct Material {
    vec3 base;
} material;
layout(location = 0) out vec3 out_position;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec4 out_diffuse;
void main(void)
{    
	out_position = frag_pos.xyz;
	out_normal   = frag_normal;
	out_diffuse =  vec4(material.base,1);
}
