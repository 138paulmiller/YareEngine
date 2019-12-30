#version 330

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D emissive;

uniform vec2 resolution;
in vec2 frag_uv;

//Write to RenderTarget 0 - See Attachment points in Rendertarget
layout(location = 0) out vec4 out_color;
void main(void)
{
	vec2 uv = frag_uv;
	out_color = texture(diffuse, uv);
	
}
