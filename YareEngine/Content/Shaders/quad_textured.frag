#version 330

uniform sampler2D color;

uniform vec2 resolution;
in vec2 frag_uv;

//Write to RenderTarget 0 - See Attachment points in Rendertarget
layout(location = 0) out vec4 out_color;
void main(void)
{    
	vec2 uv = frag_uv;
	out_color =  texture(color, uv);
}
