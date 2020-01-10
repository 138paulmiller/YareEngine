#version 330

layout(location = 0) out vec4 out_scene;
uniform float near;
uniform float far;

void main(void)
{
	out_scene = vec4(gl_FragCoord.x,0.0, 0.0,1.0);
}
