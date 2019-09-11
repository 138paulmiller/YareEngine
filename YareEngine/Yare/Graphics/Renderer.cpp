#include "Renderer.hpp"
#include "OpenGL/OpenGLRenderer.hpp"
#include "Error.hpp"

YARE_GRAPHICS_MODULE_BEG

Renderer* _renderer = 0;

Renderer* Renderer::Create(RenderAPI api)
{
	if (_renderer != 0)
	{
		YARE_ASSERT(true, "Cannot Create Renderer, Already instantied");
		return _renderer;
	}
	switch (api)
	{
	case RenderAPI::OpenGL:
		_renderer = new OpenGLRenderer();
		break;
	default:
		YARE_ASSERT(true,"Cannot Create Renderer, Invalid API");
		return 0;
	}
	_renderer->_api = api;
	return _renderer;
}

Renderer* Renderer::GetInstance()
{
	if (_renderer == 0)
	{
		YARE_ASSERT(true, "Cannot Get Renderer, Not Instatied");

	}
	return _renderer;
}




void Renderer::submit(RenderCommand * command)
{
	_commandQueue.push(command);
	
	if (_camera) 
	{
		RenderCommand * newestCommand = _commandQueue.back();
		//if a camera is bound. load it uniforms
		//use UBOs and render views for this
		newestCommand->uniformBuffer.setUniform("view", _camera->getView());
		newestCommand->uniformBuffer.setUniform("projection", _camera->getProjection());
	}
}

void Renderer::present()
{
	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	while (!_commandQueue.empty())
	{
		RenderCommand  * command = _commandQueue.front();
		_commandQueue.pop();
		
		updateState(command->state);

		command->shader->bind();
		command->uniformBuffer.load(command->shader);
		
		int i = 0;
		Texture * texture = command->textures[i];
		while (texture)
		{
			texture->bind(i);
			i++;
			texture = command->textures[i];
		}


		command->vertexArray->bind();
		switch (command->mode)
		{
		case RenderMode::IndexedMesh:
			renderIndexedMesh(command->vertexArray);
		break;
		}
		command->vertexArray->unbind();

		command->shader->unbind();
	}
}

void Renderer::beginScene(const Camera * camera)
{
	_camera = camera;
}

void Renderer::endScene()
{

	_camera = 0;
}


YARE_GRAPHICS_MODULE_END