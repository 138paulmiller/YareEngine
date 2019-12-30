#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
};

//--------------- Inputs ------------------//
in vec4 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

//--------------- Outputs ------------------//
//Write to RenderTarget 0 - See Attachment points in Rendertarget
layout(location = 0) out vec3 out_position;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec4 out_diffuse;
layout(location = 3) out vec4 out_specular;
layout(location = 4) out vec4 out_emissive;

uniform Material material;

// --------------------------- Phong ------------------------------
#define POINT_LIGHT_COUNT 64
#define DIRECTIONAL_LIGHT_COUNT 8

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 coeffs;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 view_pos;
uniform PointLight pt_lights[POINT_LIGHT_COUNT];
uniform DirectionalLight dir_lights[DIRECTIONAL_LIGHT_COUNT];

uniform int pt_light_count;
uniform int dir_light_count;

//move lighting calc to lightpass layer

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);
	//diffuse
	float diffuse_coeff = max(0.0, dot(normal, light_dir));
	//specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular_coeff = pow(max(0.0, dot(view_dir, reflect_dir)), material.shininess);
	//samples 
	vec3 ambient = light.ambient  * texture(material.diffuse, frag_uv).xyz;
	vec3 diffuse = light.diffuse  * texture(material.diffuse, frag_uv).xyz  * diffuse_coeff;
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
	float specular_coeff = diffuse_coeff * pow(clamp(dot(view_dir, reflect_dir), 0.0, 1.0), material.shininess);

	float distance = length(light.position - pos);
	//attenuation = 1/(x + d*y + d*d*z ) where coeffs = {x,y,z}
	vec3 coeffs = light.coeffs;
	float attenuation = 1.0 / (coeffs.x + coeffs.y * distance + coeffs.z * (distance * distance));

	//samples 
	vec3 ambient = light.ambient  * texture(material.diffuse, frag_uv).xyz;
	vec3 diffuse = light.diffuse  * texture(material.diffuse, frag_uv).xyz * diffuse_coeff;
	vec3 specular = light.specular * texture(material.specular, frag_uv).xyz * specular_coeff;

	return attenuation * (ambient + diffuse + specular);

}

vec4 getColor(vec3 position, vec3 view_dir, vec3 normal)
{
	vec4 color = vec4(0);

	for (int i = 0; i < dir_light_count; i++) {
		color.xyz += calcDirectionalLight(dir_lights[i], normal, view_dir);
	}

	for (int i = 0; i < pt_light_count; i++) {
		color.xyz += calcPointLight(pt_lights[i], position, normal, view_dir);
	}

	color.w = 1.0;
	return color;
}
// --------------------------- Phong End ------------------------------



void main(void)
{       
	vec3 world_position = frag_pos.xyz;

	out_position = world_position;
	out_normal = normalize(frag_normal);
	out_diffuse = texture(material.diffuse, frag_uv);
	out_specular.rgb = texture(material.specular, frag_uv).rgb;
	out_specular.a = material.shininess;
	out_emissive = texture(material.emission, frag_uv);

	vec3 view_dir = normalize(view_pos - frag_pos.xyz);
	out_diffuse = getColor(world_position, view_dir, out_normal);


}



