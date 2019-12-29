#version 330

uniform sampler2D color;
uniform sampler2D position;
uniform sampler2D normal;

uniform vec2 resolution;
in vec2 frag_uv;

//Write to RenderTarget 0 - See Attachment points in Rendertarget
layout(location = 0) out vec4 out_color;
void main(void)
{
	out_color = vec4(1.0);
	vec2 uv = frag_uv;
	vec4 c = texture(color, uv);
	vec3 p = texture(position, uv).rgb;
	vec3 n = texture(normal, uv).rgb;
	out_color.rgb = p.rgb;
	out_color.rgb += c.rgb  * 0.25;
	out_color.rgb += n.rgb * 0.25;

}
