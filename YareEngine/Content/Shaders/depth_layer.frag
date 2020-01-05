#version 330

uniform sampler2D depth;
in vec2 frag_uv;

layout(location = 0) out vec4 out_scene;

void main(void)
{
	//out_scene  = texture(depth, frag_uv);
	gl_FragDepth =  1.0 - texture(depth, frag_uv).r;
	
}
