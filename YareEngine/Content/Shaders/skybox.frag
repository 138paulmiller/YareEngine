#version 330

in vec3 uvw;

//Write to RenderTarget 0 - See Attachment points in Rendertarget

layout(location = 0) out vec4 out_scene;
uniform samplerCube environment;

void main()
{    
	out_scene = texture(environment, uvw);
	
}