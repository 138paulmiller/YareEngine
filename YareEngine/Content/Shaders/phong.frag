#version 330

// --------------------------- Defs  ------------------------------
#define POINT_LIGHT_COUNT 64
#define DIRECTIONAL_LIGHT_COUNT 8

//Create a switch for using either map or float3
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
};

struct PointLight{
	vec3 position   ;
	vec3 ambient    ;
	vec3 diffuse    ;
	vec3 specular   ;
	vec3 coeffs     ;
};

struct DirectionalLight{
	vec3 direction;
	vec3 ambient ;
	vec3 diffuse ;
	vec3 specular;
};


//--------------- Inputs ------------------//
in vec4 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

//--------------- Outputs ------------------//
out vec4 out_color;


//--------------- Uniforms ------------------//

uniform vec3 view_pos;
uniform Material material;
uniform PointLight pt_lights[POINT_LIGHT_COUNT] ;
uniform DirectionalLight dir_lights[DIRECTIONAL_LIGHT_COUNT];

uniform int pt_light_count;
uniform int dir_light_count;

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);
	//diffuse
	float diffuse_coeff = max(0.0, dot(normal, light_dir));
	//specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular_coeff = pow(max(0.0, dot(view_dir, reflect_dir)), material.shininess);
	//samples 
	vec3 ambient  = light.ambient  * texture(material.diffuse , frag_uv).xyz ;
	vec3 diffuse  = light.diffuse  * texture(material.diffuse , frag_uv).xyz  * diffuse_coeff;
	vec3 specular = light.specular * texture(material.specular, frag_uv).xyz * specular_coeff;
	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 pos, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - pos);
	//diffuse
	float diffuse_coeff = max(0.0, dot(normal, light_dir));
	
	//specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular_coeff = diffuse_coeff  * pow(clamp(dot(view_dir, reflect_dir),0.0, 1.0), material.shininess);

	float distance = length(light.position - pos);
	//attenuation = 1/(x + d*y + d*d*z ) where coeffs = {x,y,z}
	vec3 coeffs = light.coeffs;
	float attenuation = 1.0 / (coeffs.x + coeffs.y * distance + coeffs.z * (distance * distance));

	//samples 
	vec3 ambient  = light.ambient  * texture(material.diffuse , frag_uv).xyz;
	vec3 diffuse  = light.diffuse  * texture(material.diffuse , frag_uv).xyz * diffuse_coeff;
	vec3 specular = light.specular * texture(material.specular, frag_uv).xyz * specular_coeff;
	
	return attenuation * (ambient + diffuse + specular);

}


// ------------ Entry -----------------------------------
void main(void)
{       

	out_color = vec4(0);
	vec3 view_dir = normalize(view_pos - frag_pos.xyz);
	vec3 normal  = normalize(frag_normal);
	
	for(int i = 0; i < dir_light_count; i++){
		out_color.xyz += calcDirectionalLight(dir_lights[i], normal, view_dir); 
	}
	
	for(int i = 0; i < pt_light_count; i++){
		out_color.xyz += calcPointLight(pt_lights[i], frag_pos.xyz, normal, view_dir);
	}

	out_color.w=  1.0;
}
