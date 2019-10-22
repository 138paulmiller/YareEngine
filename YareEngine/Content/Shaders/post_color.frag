#version 330

uniform sampler2D color;

in vec2 frag_uv;

out vec4 out_color;
void main(void)
{    
	//out_color =  texture(color, frag_uv);
	out_color = vec4(frag_uv,1,0);;

}
