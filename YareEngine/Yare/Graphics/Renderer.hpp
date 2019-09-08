#pragma once
#include "Graphics.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
YARE_GRAPHICS_MODULE_BEG

class Renderable
{
	virtual ~Renderable()= 0;
	virtual void render(const glm::mat4& projection, const glm::mat4& view)=0;


	unsigned int _shaderId;
};

//Test functions to stencil, depth, test etc....
enum class RenderTestFunc
{
	Never =0 , //Always fails.
	Always, //Always passes
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
	Equal, 
	NotEqual
};
enum class RenderWinding {
	None =0 , Clockwise, CounterClockwise
};

enum class RenderCullFace {
	None = 0, FrontFace, BackFace
};


struct RenderConfig
{
	RenderCullFace cullFace;
	RenderWinding  winding;
	RenderTestFunc depthFunc;
	RenderTestFunc stencilFunc;
};

struct RenderCommand
{
	//RenderTarget target; //render target can have multiple attachments
	//Scene * scene;
	Camera * camera;
	Shader * shader;
	RenderConfig config;
};

//Runs on its own thread 
//all render commands should be queued
class Renderer
{
public:
	virtual ~Renderer() = default;
	virtual void render(const RenderCommand & command) = 0;
	/*
		cached config state. render command only updates if different
	*/
	RenderConfig config;
};

YARE_GRAPHICS_MODULE_END
