#pragma once
#include <stack>
#include <queue>

#include "Graphics.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

#include <unordered_map>
#include <string>

YARE_GRAPHICS_MODULE_BEG

class Renderable
{
public:
	class Renderer; //defined below
	virtual ~Renderable()= 0;
	virtual void render(const Renderer * renderer)=0;
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
enum class RenderMode {
	IndexedMesh
};

//overload == to only update if changed 
struct RenderState
{
	RenderCullFace cullFace;
	RenderWinding  winding;
	RenderTestFunc depthFunc;
	RenderTestFunc stencilFunc;
};

enum class UniformType
{
	Int, Float, Float3, Int3, Mat4

};

//Move to shader.hpp and use UBO
struct Uniform
{
	std::string name;
	union 
	{
		int i;
		float f;
		glm::vec3 float3;
		glm::dvec3 int3;
		glm::mat4 mat4;
	}
	value;
	UniformType type;
};
struct UniformBuffer
{
	std::unordered_map<std::string, Uniform> uniforms;

	void setUniform(const std::string & name, float i)
	{
		Uniform uniform;
		uniform.name = name;
		uniform.value.i = i;
		uniform.type = UniformType::Int;
		uniforms[name] = uniform;
	}
	void setUniform(const std::string & name, float f)
	{
		Uniform uniform;
		uniform.name = name;
		uniform.value.f = f;
		uniform.type = UniformType::Float;
		uniforms[name] = uniform;
	}
	void setUniform(const std::string & name, const glm::dvec3 & int3)
	{
		Uniform uniform;
		uniform.name = name;
		uniform.value.int3 = int3;
		uniform.type = UniformType::Int3;
		uniforms[name] = uniform;
	}
	void setUniform(const std::string & name, const glm::vec3 & float3)
	{
		Uniform uniform;
		uniform.name = name;
		uniform.value.float3 = float3;
		uniform.type = UniformType::Float3;
		uniforms[name] = uniform;
	}

	void setUniform(const std::string & name, const glm::mat4 & mat4)
	{
		Uniform uniform;
		uniform.name = name;
		uniform.value.mat4 = mat4;
		uniform.type = UniformType::Mat4;
		uniforms[name] = uniform;
	}


	void load(Shader * shader)
	{
		for (const std::pair<std::string, Uniform> & pair: uniforms)
		{
			switch (pair.second.type)
			{
			case UniformType::Int:
				shader->setUniform(pair.first, pair.second.value.i);
				break;
			case UniformType::Float:
				shader->setUniform(pair.first, pair.second.value.f);
				break;
			case UniformType::Float3:
				shader->setUniform(pair.first, pair.second.value.float3);
				break;
			case UniformType::Int3:
				shader->setUniform(pair.first, pair.second.value.int3);
				break;
			case UniformType::Mat4:
				shader->setUniform(pair.first, pair.second.value.mat4);
				break;
			}
		}
	}
};

#define TEXTURE_MAX 8

struct RenderCommand
{
	//Render Targets 
	//Viewport
	UniformBuffer uniformBuffer;
	VertexArray * vertexArray = 0;
	Shader * shader = 0;
	Texture * textures[TEXTURE_MAX] = { 0 };
	RenderMode mode;
};

//Runs on its own thread 
//all render commands should be queued
class Renderer
{
public:
	virtual ~Renderer() = default;
	void submit(const RenderCommand & command) ;
	void present();
	void pushState(RenderState & state);
	void popState();
	void beginScene(const Camera * camera);
	void endScene();

	virtual void renderIndexedMesh(const VertexArray * vertexArray) = 0;
	virtual void updateState(const RenderState & state) = 0;
	/*
		cached config state. render command only updates if different
	*/
private:
	std::queue<RenderCommand> _commandQueue;
	std::stack<RenderState> _stateStack;
	
	const Camera * _camera; //current 

};

YARE_GRAPHICS_MODULE_END
