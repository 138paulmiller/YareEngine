#include "Renderer.hpp"
#include "Graphics/OpenGL/OpenGLRenderer.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Light.hpp"

namespace yare { 

Renderer* Renderer::Create(RenderAPI api)
{
	Graphics::Setup(api);

	Renderer* renderer;

	switch (api)
	{
	case RenderAPI::OpenGL:
		renderer = new graphics::OpenGLRenderer();
		break;
	default:
		YARE_ASSERT(true,"Cannot Create Renderer, Invalid API");
		return 0;
	}
	return renderer;
}
void Renderer::submit(Renderable * renderable)
{
	_commandQueue.push(&renderable->command);
	
	if (_cache.scene)
	{
		RenderCommand * newestCommand = _commandQueue.back();
		//if a scene is bound. load it uniforms
		//use UBOs and render views for this
		_cache.scene->loadUniforms(newestCommand->uniforms, newestCommand->lighting);
	}
}

void Renderer::render()
{
	RenderCommand *command;
	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	while (!_commandQueue.empty())
	{
		command = _commandQueue.front();
		_commandQueue.pop();
		
		if (_cache.target)
		{
			_cache.target->bind();
		}


		updateState(command->state);
		static Shader* prevShader = nullptr;
		if (prevShader != command->shader) {

			command->shader->bind();
			prevShader = command->shader;
		}
		/*
		Instead of loading all uniforms and texture each frame. Create Shader Instances that are copies of a base shader. 
		Will only have to bind when dirty. Since they are copies, they should not be affected by other renderables that share the same parent shader
		Potentially do this at the material level and have materials instances maintain *blocks.
		*/

		command->uniforms.load(command->shader);
		command->textures.load(command->shader);

		command->vertexArray->bind();
		
		switch (command->mode)
		{
		case RenderMode::Mesh:
			renderMesh(command->vertexArray);
			break;
		case RenderMode::IndexedMesh:
			renderIndexedMesh(command->vertexArray);
		break;
		}

		command->vertexArray->unbind();
	}
}

void Renderer::begin(Scene* scene, RenderTarget * target)
{
	_cache.scene = scene;
	_cache.target = target;
}

void Renderer::end()
{
	_cache.scene = 0;
	_cache.target = 0;
}


} 