#pragma once
#include "Graphics.hpp"
#include "Camera.hpp"
YARE_GRAPHICS_MODULE_BEG

class Renderable
{
	virtual ~Renderable()= 0;
	virtual void render(const glm::mat4& projection, const glm::mat4& view)=0;


	unsigned int _shaderId;
};


struct RenderCommand
{
	//camera, shader, scene[octree of renderables], framebuffer, options (faceculling etc...) 

};

//Runs on its own thread 
//all render commands should be queued
class Renderer
{
public:
	virtual ~Renderer() = default;
	virtual void render(const RenderCommand & command) = 0;


	//add cull face, depoth test vars etc...
};

YARE_GRAPHICS_MODULE_END
