#version 330

in vec3 uvw;

//Write to RenderTarget 0 - See Attachment points in Rendertarget

layout(location = 0) out vec3 out_position;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec4 out_diffuse;
uniform samplerCube environment;

void main()
{    
	out_diffuse = texture(environment, uvw);
	
}