#version 330

//world to light space
uniform mat4 light_space;

uniform sampler2D shadowmap;


in vec2 frag_uv;
in vec4 light_space_pos;


layout(location = 0) out vec4 out_scene;
void main(void)
{
	//calculate if in shadow or not
	//perspective divide, event tho it should be ortho
	vec3 proj_coords = light_space_pos.xyz / light_space_pos.w;
	proj_coords = proj_coords * 0.5 + 0.5;

	//nearest depth from light perpective
	float nearest_depth = texture(shadowmap, proj_coords.xy).r;
	//depth of fragment from light perspective
	float current_depth = proj_coords.z;

	float shadow = current_depth > nearest_depth ? 255.0 : 0.0;
	out_scene = vec4(shadow);

}
