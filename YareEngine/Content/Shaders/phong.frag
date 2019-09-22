#version 330

//--------------- Inputs ------------------//
in vec4 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

//--------------- Outputs ------------------//
out vec4 color;


//--------------- Uniforms ------------------//
uniform vec3 view_pos;


//Create a switch for using either map or float3
uniform struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
} material;


#define POINT_LIGHT_COUNT 64
#define DIRECTIONAL_LIGHT_COUNT 64

struct PointLight{
	vec3 position   ;
	vec3 ambient    ;
	vec3 diffuse    ;
	vec3 specular   ;
	vec3 attenuation;
};
uniform PointLight pt_lights[POINT_LIGHT_COUNT] ;

struct DirectionalLight{
	vec3 direction;
	vec3 ambient ;
	vec3 diffuse ;
	vec3 specular;
};

uniform DirectionalLight dir_lights[DIRECTIONAL_LIGHT_COUNT];

// ------------ Functions -----------------------------------

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);
	//diffuse
	float diffuse_coeff = max(0.0, dot(normal, light_dir));
	//specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular_coeff = pow(max(0.0, dot(view_dir, reflect_dir)), material.shininess);
	//samples 
	vec3 ambient  = light.ambient * texture(material.diffuse, frag_uv).xyz;
	vec3 diffuse  = light.diffuse * diffuse_coeff * texture(material.diffuse, frag_uv).xyz;
	vec3 specular = light.specular * specular_coeff * texture(material.specular , frag_uv).xyz;
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
	//attenuation = 1/(x + d*y + d*d*z ) where values = {x,y,z}
	vec3 values = light.attenuation;
	float attenuation = 1.0 / (values.x + values.y * distance +  values.z * (distance * distance));

	//samples 
	vec3 ambient  = light.ambient * texture(material.diffuse, frag_uv).xyz;
	vec3 diffuse  = light.diffuse * diffuse_coeff * texture(material.diffuse, frag_uv).xyz;
	vec3 specular = light.specular * specular_coeff * texture(material.specular , frag_uv).xyz;
	//TODO fixme
	//ambient  *= attenuation;
    //diffuse  *= attenuation;
    //specular *= attenuation;

	return ambient + diffuse + specular;

}


// ------------ Entry -----------------------------------
void main(void)
{       
    vec3 view_dir = normalize(view_pos - frag_pos.xyz);
	vec3 normal  = normalize(frag_normal);
	/*
	for(int i = 0; i < DIRECTIONAL_LIGHT_COUNT; i++){
		color.xyz += calcDirectionalLight(dir_lights[i], normal, view_dir); 
	}
	*/
	
	for(int i = 0; i < POINT_LIGHT_COUNT; i++){
		color.xyz += calcPointLight(pt_lights[i], frag_pos.xyz, normal, view_dir);
	}

	color.w=  1.0;
}
