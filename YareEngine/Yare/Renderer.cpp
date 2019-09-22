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
	_renderQueue.push(renderable);
	
	if (_scene)
	{
		Renderable  * newestRenderable = _renderQueue.back();
		//if a scene is bound. load it uniforms
		//use UBOs and render views for this
		_scene->loadUniforms(newestRenderable->renderData.uniforms, newestRenderable->renderData.lighting);		
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

		/*
		Instead of loading all uniforms and texture each frame. Create Shader Instances that are copies of a base shader. 
		However, will only have to bind when dirty. Since they are copies, they should not be affected by other renderables that share the same parent shader
		Potentially do this at the material level and have materials instances maintain *blocks.
		*/

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

void Renderer::beginScene(const Scene* scene)
{
	_scene= scene;
}

void Renderer::endScene()
{

	_scene = 0;
}


} 