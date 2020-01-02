#version 330

uniform sampler2D color;
uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D depth;
uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D emissive;
uniform vec2 resolution;
in vec2 frag_uv;

void main(void)
{
	
	gl_FragDepth = texture(depth, frag_uv);
	
}
