#version 330

in vec4 frag_pos;
in vec3 frag_norm;
in vec2 frag_uv;

in vec4 light_pos;
in vec4 view_pos;

//Create a switch for using either map or float3
uniform struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
} material;


// output
out vec4 color;

void main(void)
{       
    //diffuse coef
	vec3 light_dir = normalize(light_pos.xyz - frag_pos.xyz);
	float diff_coef = max(dot(frag_norm, light_dir), 0.0);
	
	//specular coef
	vec3 view_dir = normalize(view_pos.xyz - frag_pos.xyz);
    vec3 reflect_dir = reflect(-light_dir, frag_norm);
    float spec_coef = pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);
    
	
	vec3 diffuse =  diff_coef * vec3(texture(material.diffuse, frag_uv));   
	vec3 specular = spec_coef * vec3(texture(material.specular, frag_uv));    
	
    color.xyz =  diffuse + specular;
}
