#version 330

in vec4 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

in vec4 view_pos;

//Create a switch for using either map or float3
uniform struct Material {
    vec3 base;
} material;

out vec4 color;
void main(void)
{    
	color =  vec4(material.base,1);
}
