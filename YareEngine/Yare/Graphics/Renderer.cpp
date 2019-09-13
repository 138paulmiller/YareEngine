#include "Renderer.hpp"
#include "OpenGL/OpenGLRenderer.hpp"
#include "Error.hpp"

namespace yare { namespace graphics {  




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




void Renderer::submit(Renderable * renderable)
{
	_renderQueue.push(renderable);
	
	if (_camera) 
	{
		Renderable  * newestRenderable = _renderQueue.back();
		//if a camera is bound. load it uniforms
		//use UBOs and render views for this
		newestRenderable->renderData.uniformBlock.setUniform("view", _camera->getView());
		newestRenderable->renderData.uniformBlock.setUniform("projection", _camera->getProjection());
	}
}

void Renderer::present()
{
	Renderable* renderable;
	RenderData *data;
	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	while (!_renderQueue.empty())
	{


		renderable = _renderQueue.front();
		data = &renderable->renderData;
		_renderQueue.pop();
		
		renderable->preRender();
		updateState(data->state);

		data->shader->bind();
		data->uniformBlock.load(data->shader);
		data->textureBlock.load(data->shader);


		data->vertexArray->bind();
		
		switch (data->mode)
		{
		case RenderMode::IndexedMesh:
			renderIndexedMesh(data->vertexArray);
		break;
		}

		data->vertexArray->unbind();
		data->shader->unbind();

		renderable->postRender();
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


} } 