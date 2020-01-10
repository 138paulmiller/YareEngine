#version 330

// --------------------------- Defs  ------------------------------
#define POINT_LIGHT_COUNT 64
#define DIRECTIONAL_LIGHT_COUNT 8


struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 attenuation;

	float radius;
	int cast_shadow;
	sampler3D shadowmap; //
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float radius;
	int cast_shadow;
	sampler2D shadowmap; //
};
//shade is measured as 1.0/TOTAL_LIGHTCOUNT

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D depth;
uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D emissive;


uniform PointLight pt_lights[POINT_LIGHT_COUNT];
uniform DirectionalLight dir_lights[DIRECTIONAL_LIGHT_COUNT];



uniform vec2 resolution;
uniform vec3 view_pos;

uniform int pt_light_count;
uniform int dir_light_count;

in vec2 frag_uv;
//move lighting calc to lightpass layer

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);
	//diffuse
	float diffuse_coeff = max(0.0, dot(normal, light_dir));
	//specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float shininess = texture(specular, frag_uv).a;
	float specular_coeff = pow(max(0.0, dot(view_dir, reflect_dir)), shininess);
	//samples 
	vec3 a = light.ambient  * texture(diffuse, frag_uv).xyz;
	vec3 d = light.diffuse  * texture(diffuse, frag_uv).xyz  * diffuse_coeff;
	vec3 s = light.specular * texture(specular, frag_uv).xyz * specular_coeff;
	return a + d + s;
}

vec3 calcPointLight(PointLight light, vec3 pos, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - pos);
	//diffuse
	float diffuse_coeff = max(0.0, dot(normal, light_dir));

	//specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float shininess = texture(specular, frag_uv).a;

	float specular_coeff = diffuse_coeff * pow(clamp(dot(view_dir, reflect_dir), 0.0, 1.0), shininess);

	float distance = length(light.position - pos);
	//attenuation = 1/(x + d*y + d*d*z ) where coeffs = {x,y,z}
	vec3 coeffs = light.attenuation;
	float attenuation = 1.0 / (coeffs.x + coeffs.y * distance + coeffs.z * (distance * distance));

	//samples 
	vec3 a = light.ambient  * texture(diffuse, frag_uv).xyz;
	vec3 d = light.diffuse  * texture(diffuse, frag_uv).xyz * diffuse_coeff;
	vec3 s = light.specular * texture(specular, frag_uv).xyz * specular_coeff;

	return attenuation * (a + d + s);

}

vec4 getColor(vec3 world_position, vec3 view_dir, vec3 normal)
{
	vec4 color = vec4(0);
	for (int i = 0; i < dir_light_count; i++) {
		color.xyz += calcDirectionalLight(dir_lights[i], normal, view_dir);
	}

	for (int i = 0; i < pt_light_count; i++) {
		color.xyz += calcPointLight(pt_lights[i], world_position, normal, view_dir);
	}

	color.w = 1.0;
	return color;
}

// ------------ Entry -----------------------------------
//Write to RenderTarget 0 - See Attachment points in Rendertarget
layout(location = 0) out vec4 out_scene;
void main(void)
{

	vec3 position = texture(position, frag_uv).rgb;
	vec3 normal = texture(normal, frag_uv).rgb;


	vec3 view_dir = normalize ( view_pos - position.xyz ) ;
	out_scene = getColor(position, view_dir, normal);

}
