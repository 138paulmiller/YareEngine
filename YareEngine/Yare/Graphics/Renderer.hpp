#pragma once
#include "Graphics.hpp"
YARE_GRAPHICS_MODULE_BEG

struct RenderCommand
{
	//camera, shader, scene, framebuffer, options (faceculling etc...) 

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
