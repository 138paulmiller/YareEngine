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
	setupRenderPasses();
}
Renderer::~Renderer()
{
	for (RenderBuffers::iterator it = _targets.begin();it != _targets.end(); it++) {
		delete *it;
		*it = 0;
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

void Renderer::submit(Renderable * renderable)
{
	RenderCommand * command  = &renderable->command;
	if (_cache.scene)
	{
		//if a scene is bound. load it uniforms
		//use UBOs and render views for this
		_cache.scene->unloadUniforms(command->uniforms);
	}
	switch (renderable->command.lighting)
	{
		case RenderLighting::Surface:
			//surface, add commands to 
			_passes[(int)RenderPass::Geometry].commands.push_back(command);
			break;
		case RenderLighting::Unlit:
			_passes[(int)RenderPass::Geometry].commands.push_back(command);
			//_passes[(int)RenderPass::Forward].commands.push_back(command);
			break;

	}
	

}
void Renderer::end()
{
	_cache.scene = 0;
}



void Renderer::setupRenderPasses()
{
	for (int i = 0; i < (const int)RenderPass::Count; i++) {
		_passes[i].target = 0;
		_passes[i].sampleRate = 1.0;
	}

	RenderTarget * gbuffer = RenderTarget::Create();
	gbuffer->setup({
		RenderTargetAttachment::Position,
		RenderTargetAttachment::Normal,
		RenderTargetAttachment::Diffuse,
		RenderTargetAttachment::Specular,
		RenderTargetAttachment::Emissive,
		}
	);
	_targets.push_back(gbuffer);
	
	RenderTarget * color = RenderTarget::Create();
	color->setup({
		RenderTargetAttachment::Position,
		RenderTargetAttachment::Normal,
		RenderTargetAttachment::Diffuse,
		}
	);
	_targets.push_back(color);

	//Setup geometry pass
	RenderPassCommand & geometryPass = _passes[(const int)RenderPass::Geometry];
	geometryPass.render = &Renderer::renderGeometryPass;
	geometryPass.target = gbuffer;

	
	//Setup lighting pass
	RenderPassCommand & lightingPass = _passes[(const int)RenderPass::Lighting];
	lightingPass.render = &Renderer::renderLightingPass;
	lightingPass.inputs = { geometryPass.target };//default framebuffer;
//	lightingPass.target = color;//uses gbuffers depth buffer 

	//Setup forward pass
	RenderPassCommand & forwardPass = _passes[(const int)RenderPass::Forward];
	forwardPass.render = &Renderer::renderForwardPass;
	forwardPass.inputs = { geometryPass.target };//uses gbuffers depth buffer 
//	forwardPass.target = color;//uses gbuffers depth buffer 

}

//Enable forward rendering, for all unlit, render these to the screen unlit.
//the render deferred passes 
void Renderer::render()
{

	renderPass(RenderPass::Geometry);
	renderPass(RenderPass::Lighting);
	// renderPass(RenderPass::Forward);

	for(int i = 0; i < (const int)RenderPass::Count; i++)
		_passes[i].commands.clear();

	//renderColor();
}


void Renderer::renderPass(RenderPass pass)
{
	RenderPassCommand & passCommand = _passes[(int)pass];
	//Resize target to match desired dimensions
	if (passCommand.target) {
		//bind the target to render to
		passCommand.target->bind();
		if (passCommand.target->getWidth() != _width || passCommand.target->getHeight() != _height) {
			passCommand.target->resize(_width * passCommand.sampleRate, _height* passCommand.sampleRate);
		}

	}
	//render
	if(passCommand.render)
	(this->* (passCommand.render) )(passCommand);

	//unbind the rendered target
	if (passCommand.target)passCommand.target->unbind();
	
}


void Renderer::renderCommands(const std::vector<RenderCommand * > & commands)
{
	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	for (const RenderCommand * command : commands)
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

void Renderer::renderGeometryPass(const RenderPassCommand & pass)
{

	this->clear(RenderBufferFlag::Color);
	this->clear(RenderBufferFlag::Depth);
	renderCommands(pass.commands);

}


void  Renderer::renderLightingPass(const RenderPassCommand & pass)
{
	this->clear(RenderBufferFlag::Color);
	this->clear(RenderBufferFlag::Depth);

	RenderState layersState; //default state
	layersState.cullFace = RenderCullFace::Back;
	layersState.depthFunc = RenderTestFunc::Disabled; 
	updateState(layersState);

	Layer * _layer = new Layer();

	Shader * layerShader = AssetManager::GetInstance().get<Shader>("phong_layer");
	_cache.scene->unloadUniforms(_layer->getUniforms());
	_layer->setQuad({ -1,-1 }, { 1, 1 });
	_layer->setShader(layerShader);
	_layer->getInputs().clear();
	_layer->getInputs().insert(_layer->getInputs().begin(), pass.inputs.begin(), pass.inputs.end());
	_layer->setTarget(pass.target);
	_layer->render(this);

	delete _layer;

}
void  Renderer::renderForwardPass(const RenderPassCommand & pass)
{

	pass.inputs[0]->copyDepthBuffer(pass.target);
	renderCommands(pass.commands);

}


void  Renderer::renderColor()
{

	RenderState layersState; //default state
	layersState.cullFace = RenderCullFace::Back;
	layersState.depthFunc = RenderTestFunc::Disabled;
	updateState(layersState);

	Layer * _layer = new Layer();

	Shader * layerShader = AssetManager::GetInstance().get<Shader>("unlit_layer");
	_cache.scene->unloadUniforms(_layer->getUniforms());
	_layer->setQuad({ -1,-1 }, { 1, 1 });
	_layer->setShader(layerShader);
	_layer->getInputs().clear();

	_layer->addInput(_passes[(int)RenderPass::Forward].target);
	_layer->render(this);

	delete _layer;

}
} 