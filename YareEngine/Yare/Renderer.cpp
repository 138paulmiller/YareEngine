#include "Renderer.hpp"
#include "Graphics/OpenGL/OpenGLRenderer.hpp"
#include "Graphics/Graphics.hpp"
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
	renderer->_api = api;
	return renderer;
}
void Renderer::submit(Renderable * renderable)
{
	_renderQueue.push(renderable);
	
	if (_camera) 
	{
		Renderable  * newestRenderable = _renderQueue.back();
		//if a camera is bound. load it uniforms
		//use UBOs and render views for this
		newestRenderable->renderData.uniforms.setUniform("view", _camera->getView());
		newestRenderable->renderData.uniforms.setUniform("projection", _camera->getProjection());
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
		data->uniforms.load(data->shader);
		data->textures.load(data->shader);


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

void Renderer::beginScene(const graphics::Camera * camera)
{
	_camera = camera;
}

void Renderer::endScene()
{

	_camera = 0;
}


} 