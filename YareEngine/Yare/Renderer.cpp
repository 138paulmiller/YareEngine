#include "Renderer.hpp"
#include "Graphics/OpenGL/OpenGLRenderer.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Light.hpp"


//Remove 
#include "AssetManager.hpp"
#include "Graphics/OpenGL/OpenGLError.hpp"
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


Renderer::Renderer()
{
	for (int i = 0; i < (const int)RenderPass::Count; i++) {
		_passes[i].target = 0;
	}
	
	RenderTarget * gbuffer = RenderTarget::Create();
	gbuffer->use({
		RenderTargetAttachment::Position,
		RenderTargetAttachment::Normal,
		RenderTargetAttachment::Diffuse,
		RenderTargetAttachment::Specular,
		RenderTargetAttachment::Emissive,
		}
	);
	
	_passes[(const int)RenderPass::Geometry].target = gbuffer;
}
Renderer::~Renderer()
{
	for (int i = 0; i < (const int)RenderPass::Count; i++) {
		delete _passes[i].target;
	}

}

void Renderer::resizeViewport(int width, int height)
{
	_width = width;
	_height = height;
}

void Renderer::begin(Scene* scene)
{
	_cache.scene = scene;

	//Init layers

}

void Renderer::submit(Renderable * renderable, RenderPass pass)
{

	_passes[(int)pass].commands.push_back(&renderable->command);
	
	if (_cache.scene)
	{
		RenderCommand * newestCommand = _passes[(int)pass].commands.back();
		//if a scene is bound. load it uniforms
		//use UBOs and render views for this
		_cache.scene->loadUniforms(newestCommand->uniforms);
	}
}
void Renderer::end()
{
	_cache.scene = 0;
}

//Enable forward rendering, for all unlit, render these to the screen unlit.
//the render deferred passes 
void Renderer::render()
{
	RenderTarget * gbuffer;	
	for (int i = 0; i < (const int)RenderPass::Count; i++) 
	{
		switch (RenderPass(i)) 
		{
		case RenderPass::Geometry:
			renderGeometry(_passes[i]);
			gbuffer = _passes[i].target; //current target being rendered to
			_passes[i].commands.clear();
			break;
		default://do nothing
			break;
		}
	}
	//First Render Unlit Commands
	//Then render lit command
	//Move this a custom pass. 
	{
		RenderState layersState; //default state
		layersState.cullFace = RenderCullFace::Back;
		layersState.depthFunc = RenderTestFunc::Disabled;
		updateState(layersState);

		Layer * _layer = new Layer();

		Shader * layerShader = AssetManager::GetInstance().get<Shader>("phong_layer");
		UniformBlock uniforms;
		_cache.scene->loadUniforms(uniforms);

		layerShader->bind();
		uniforms.load(layerShader);
		layerShader->unbind();
		_layer->setQuad({ -1,-1 }, { 1, 1 });
		_layer->setShader(layerShader);

		_layer->addInput(gbuffer);

		_layer->render(this);
		delete _layer;
	}
}
void Renderer::renderGeometry(const RenderPassCommand & pass)
{

	if (pass.target) {
		if (pass.target->getWidth() != _width || pass.target->getHeight() != _height) {
			pass.target->resize(_width * pass.sampleRate, _height* pass.sampleRate);
		}
		pass.target->bind();
	}
	
	this->clear(RenderBufferFlag::Color);
	this->clear(RenderBufferFlag::Depth);


	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	for(RenderCommand const * command : pass.commands)
	{		
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

	if (pass.target)pass.target->unbind();
}




} 