#version 330 core

//Create a switch for using either map or float3
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
};
uniform Material material;


// ------------ Entry -----------------------------------
void main(void)
{       
	vec3 view_dir = normalize(view_pos - frag_pos.xyz);
	
	out_position = frag_pos.xyz;
	out_normal = normalize(frag_normal);
	out_color = material.diffuse;

}
