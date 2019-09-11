#pragma once
#include <stack>
#include <queue>

#include "Error.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "UniformBuffer.hpp"



#define DEFAULT_RENDER_API RenderAPI::OpenGL
#define TEXTURE_MAX 8

YARE_GRAPHICS_MODULE_BEG

enum class RenderAPI
{
	None=0, OpenGL
};


//Test functions to stencil, depth, test etc....
enum class RenderTestFunc
{
	Less=0,
	LessEqual,
	Greater,
	GreaterEqual,
	Equal, 
	NotEqual,
	Never, //Always fails.
	Always //Always passes
};

enum class RenderWinding {
	Clockwise=0, CounterClockwise
};

enum class RenderCullFace {
	Back = 0, Front
};

enum class RenderPrimitive {
	Triangles=0
};


enum class RenderMode {
	IndexedMesh = 0
};

//overload == to only update if changed 
struct RenderState
{
	RenderCullFace cullFace;
	RenderWinding  winding;
	RenderTestFunc depthFunc;
	RenderTestFunc stencilFunc;
};



struct RenderCommand
{
	//Render Targets 
	//Viewport
	UniformBuffer uniformBuffer;
	VertexArray * vertexArray = 0;
	Shader * shader = 0;
	Texture * textures[TEXTURE_MAX] = { 0 };
	RenderMode mode;
	RenderPrimitive primitive;
};



class Renderer;

class Renderable
{
public:
	virtual ~Renderable() = default;
	virtual void render(Renderer* renderer) = 0;
};

//Runs on its own thread 
//all render commands should be queued
class Renderer
{
public:

	static Renderer* Create(RenderAPI api);
	static Renderer *GetInstance();

	virtual ~Renderer() = default;
	void submit(const RenderCommand & command) ;
	void present();
	void pushState(RenderState & state);
	void popState();
	void beginScene(const Camera * camera);
	void endScene();
	inline RenderAPI getAPI() { return _api; }

protected:
	virtual void renderIndexedMesh(const VertexArray * vertexArray) = 0;
	virtual void updateState(const RenderState & state) = 0;
	/*
		cached config state. render command only updates if different
	*/
private:
	std::queue<RenderCommand> _commandQueue;
	std::stack<RenderState> _stateStack;
	
	const Camera * _camera; //current 
	RenderAPI _api;
};

YARE_GRAPHICS_MODULE_END
