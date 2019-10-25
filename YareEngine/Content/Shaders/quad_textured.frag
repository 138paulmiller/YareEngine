#version 330

uniform sampler2D color;

uniform vec2 resolution;
in vec2 frag_uv;

out vec4 out_color;
void main(void)
{    
	//vec2 uv = vec2 (frag_uv.x*resolution.x, frag_uv.y* resolution.y)/gl_FragCoord;
	out_color =  texture(color, frag_uv);
	//out_color = vec4(frag_uv,1,0);;

}
