#version 330

// --------------------------- Defs  ------------------------------
#define POINT_LIGHT_COUNT 64
#define DIRECTIONAL_LIGHT_COUNT 8

struct PointLight {
	vec3 position;
	float radius;
	int cast_shadow;
	sampler3D shadowmap; //

	//world to light space
	mat4 light_space;
};

struct DirectionalLight {
	vec3 direction;
	float radius;
	int cast_shadow;
	//depth map
	sampler2D shadowmap; //
	//world to light space
	mat4 light_space;
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

in vec2 frag_uv;

uniform vec2 resolution;
uniform vec3 view_pos;

uniform int pt_light_count;
uniform int dir_light_count;

//move lighting calc to lightpass layer


vec3 calcDirectionalLightShadow(DirectionalLight light, vec3 world_position)
{
	if (light.cast_shadow == 0) {
		return vec3(0.0);
	}
	vec4 light_space_pos = light.light_space * vec4(world_position, 1.0);
	vec3 proj_coords = light_space_pos.xyz / light_space_pos.w; //perpective divide
	//-1,1 -> 0,1
	proj_coords = proj_coords * 0.5 + 0.5;
	//nearest depth to light
	float nearest_depth = texture(light.shadowmap, proj_coords.xy).r;
	return vec3(nearest_depth, nearest_depth, nearest_depth);
}

vec3 calcPointLightShadow(PointLight light, vec3 world_position)
{
	return vec3(0.0);

}

vec4 getShade(vec3 world_position)
{
	vec4 color = vec4(0);
	for (int i = 0; i < dir_light_count; i++) {
		color.xyz += calcDirectionalLightShadow(dir_lights[i], world_position);
		
	}

	for (int i = 0; i < pt_light_count; i++) {
		color.xyz += calcPointLightShadow(pt_lights[i], world_position);
	}

	color.w = 1.0;
	return color;
}

// ------------ Entry -----------------------------------
//Write to RenderTarget 0 - See Attachment points in Rendertarget
layout(location = 0) out vec4 out_scene;
void main(void)
{
	out_scene = vec4(1.0);
	vec3 world_position = texture(position, frag_uv).xyz;
	out_scene.r = getShade(world_position).r;

}
