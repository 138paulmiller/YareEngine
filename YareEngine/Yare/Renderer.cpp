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


void Renderer::begin(Scene* scene)
{
	_cache.scene = scene;

	//Init layers

}

void Renderer::submit(Renderable * renderable)
{
	_commands.push_back(&renderable->command);
	
	if (_cache.scene)
	{
		RenderCommand * newestCommand = _commands.back();
		//if a scene is bound. load it uniforms
		//use UBOs and render views for this
		_cache.scene->loadUniforms(newestCommand->uniforms, newestCommand->lighting);

	}
}
void Renderer::end()
{
	_cache.scene = 0;
}

#define DEFERRED_DBG
void Renderer::render()
{
#ifdef DEFERRED_DBG
	///////////// Debug Render Layers ////////////////////////////////////////
	RenderTarget *target = RenderTarget::Create();
	target->use(RenderTargetAttachment::Color);
	target->resize(1920, 1260); //should send screen resolution to shader
	target->setup();
	target->bind();

	this->clear(RenderBufferFlag::Color);
	this->clear(RenderBufferFlag::Depth);
	
	if (_commands.empty()) return;
	renderGeometry(_commands);
	_commands.clear();
	target->unbind();
	//this->clear(RenderBufferFlag::Color);
	
	
	RenderState layersState; //default state
	layersState.cullFace = RenderCullFace::Back;
	layersState.depthFunc = RenderTestFunc::Disabled;
	updateState(layersState);
	OpenGLCheckError();

	_layer = new Layer();
	OpenGLCheckError();

	Shader * layerShader = AssetManager::GetInstance().get<Shader>("quad_textured");
	OpenGLCheckError();

	_layer->setQuad({ 0,0 }, { 1, 1 });
	OpenGLCheckError();
	_layer->setShader(layerShader);
	OpenGLCheckError();

	layerShader->setUniform("color", 0);
	//layerShader->setUniform("resolution", glm::vec2(target->getWidth(), target->getHeight()));
	OpenGLCheckError();
	_layer->setTarget(target);
	
	OpenGLCheckError();

	_layer->render(this);
	delete _layer;
	delete target;
#else
	////////////////////////////////////////////////////////////////////////////
	if (_commands.empty()) return;
	renderGeometry(_commands);
	_commands.clear();
#endif

#undef DEFERRED_DBG
}
void Renderer::renderGeometry(const std::vector<RenderCommand * > & commands)
{
	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	for(RenderCommand const * command : commands)
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
}




} 