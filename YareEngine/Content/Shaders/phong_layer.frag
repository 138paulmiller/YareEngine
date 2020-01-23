#version 330

// --------------------------- Defs  ------------------------------
//Move this to common
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
	mat4 view_proj;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float radius;
	int cast_shadow;
	sampler2D shadowmap; //
	mat4 view_proj;
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

//
float calcShadowAmount(sampler2D shadowmap, mat4 light_space, vec3 world_pos, float bias)
{

	//calculate if in shadow or not
	vec4 view_proj_pos = light_space * vec4(world_pos, 1.0); 
	
	//perspective divide, event tho it should be ortho
	vec3 proj_coords = view_proj_pos.xyz  / view_proj_pos.w;
	//screen to texture space -1,1 to 0-1 
	proj_coords = proj_coords * 0.5 + 0.5;

	//nearest depth from light perpective
	float nearest_depth = texture(shadowmap, proj_coords.xy).r;
	//depth of fragment from light perspective
	float current_depth = proj_coords.z;
	
	float shadow = 0.0;
	//pixel is occluded
	if( (current_depth + bias) > nearest_depth)
	{
		shadow = 1.0;
	}
	return shadow;
}

vec3 calcDirectionalLightRadiance(DirectionalLight light, vec3 world_pos, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);
	//diffuse
	float diffuse_coeff = max(0.0, dot(normal, light_dir));
	//specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float shininess = texture(specular, frag_uv).a;
	float specular_coeff = pow(max(0.0, dot(view_dir, reflect_dir)), shininess);
	
	float bias = 0.0001;
	//bias toward light
	bias = max((bias + 0.001) * (1.0 - dot(normal, light_dir)), bias);

	float shadow = calcShadowAmount(light.shadowmap, light.view_proj, world_pos, bias);

	//samples 
	vec3 a = light.ambient  * texture(diffuse, frag_uv).xyz;
	vec3 d = light.diffuse  * texture(diffuse, frag_uv).xyz  * diffuse_coeff;
	vec3 s = light.specular * texture(specular, frag_uv).xyz * specular_coeff;
	return a + (d + s) * (1.0-shadow);
}

vec3 calcPointLightRadiance(PointLight light, vec3 pos, vec3 normal, vec3 view_dir)
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
		color.xyz += calcDirectionalLightRadiance(dir_lights[i], world_position, normal, view_dir);
	}

	for (int i = 0; i < pt_light_count; i++) {
		color.xyz += calcPointLightRadiance(pt_lights[i], world_position, normal, view_dir);
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
	vec3 depth= texture(depth, frag_uv).rgb;


	vec3 view_dir = normalize ( view_pos - position.xyz ) ;
	out_scene = getColor(position, view_dir, normal);

}
