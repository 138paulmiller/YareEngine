#version 330

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D emissive;

uniform vec2 resolution;
in vec2 frag_uv;

layout(location = 0) out vec3 out_position;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec4 out_diffuse;
void main(void)
{
	vec2 uv = frag_uv;

	out_position = texture(position, uv).rgb;
	out_normal = texture(normal,uv).rgb;
	out_diffuse = texture(diffuse, uv);

	
}
