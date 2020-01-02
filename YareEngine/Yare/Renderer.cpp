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
	_width = _height = 0;
	_cache.scene = 0;
	setupRenderPasses();
}
Renderer::~Renderer()
{
	for (RenderBuffers::iterator it = _targets.begin();it != _targets.end(); it++) {
		delete it->second;
		it->second;
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
			//_passes[(int)RenderPass::Geometry].commands.push_back(command);
			_passes[(int)RenderPass::Forward].commands.push_back(command);
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
	}

	RenderTarget * gbuffer = RenderTarget::Create();
	gbuffer->setup({
		RenderTargetAttachment::Position,
		RenderTargetAttachment::Normal,
		RenderTargetAttachment::Diffuse,
		RenderTargetAttachment::Specular,
		RenderTargetAttachment::Emissive,
		//should use depth

		}
	);
	//glsl must write to scene
	RenderTarget* scene = RenderTarget::Create();
	scene->setup({
		RenderTargetAttachment::Scene,
		//should use depth
		}
	);

	_targets["gbuffer"] = gbuffer;
	_targets["scene"] = scene;


	//Setup geometry pass
	RenderPassCommand & geometryPass = _passes[(const int)RenderPass::Geometry];
	geometryPass.render = &Renderer::renderGeometryPass;
	geometryPass.targetScalar = 1.0;
	geometryPass.target = gbuffer;

	float sceneBufferScalar = 1.0 / 2.0;
	//Setup lighting pass
	RenderPassCommand & lightingPass = _passes[(const int)RenderPass::Lighting];
	lightingPass.targetScalar = sceneBufferScalar;

	lightingPass.render = &Renderer::renderLightingPass;
	lightingPass.inputs = { geometryPass.target };//default framebuffer;
	lightingPass.target = scene;
	
	//Setup forward pass
	RenderPassCommand & forwardPass = _passes[(const int)RenderPass::Forward];
	forwardPass.targetScalar = sceneBufferScalar;

	forwardPass.render = &Renderer::renderForwardPass;
	forwardPass.inputs = { geometryPass.target };//uses gbuffers depth buffer 
	forwardPass.target = scene;

	//Setup forward pass
	RenderPassCommand& scenePass = _passes[(const int)RenderPass::Scene];
	scenePass.targetScalar = sceneBufferScalar;

	scenePass.render = &Renderer::renderScenePass;
	scenePass.inputs = { scene };//uses gbuffers depth buffer 
	scenePass.target = 0;//uses gbuffers depth buffer 


}

//Enable forward rendering, for all unlit, render these to the screen unlit.
//the render deferred passes 
void Renderer::render()
{
	
	renderPass(RenderPass::Geometry);
	renderPass(RenderPass::Lighting);
	renderPass(RenderPass::Forward);

	//render post processes, initial post process should use scene as input. chaiun outputs into inputs
	renderPass(RenderPass::Scene);
	//as of now, only geometry passa and forward use commands. all other are just layer draws of buffer copies
	for(int i = 0; i < (const int)RenderPass::Count; i++)
		_passes[i].commands.clear();

}


void Renderer::renderPass(RenderPass pass)
{
	RenderPassCommand & passCommand = _passes[(int)pass];
	//Resize target to match desired dimensions
	if (passCommand.target) {
		//bind the target to render to
		int w = _width ;
		int h = _height;
		if (passCommand.target->getWidth() != w || passCommand.target->getHeight() != h) {
			passCommand.target->resize(w * passCommand.targetScalar, h * passCommand.targetScalar);
		}								  
		passCommand.target->bind();
		resizeViewport(passCommand.target->getWidth(), passCommand.target->getHeight());

	}

	//render
	if(passCommand.render)
	(this->* (passCommand.render) )(passCommand);

	if (passCommand.target)
		passCommand.target->unbind();


	//unbind the rendered target
	resizeViewport(_width, _height);
}


void Renderer::renderCommands(const std::vector<RenderCommand * > & commands)
{
	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	for (const RenderCommand * command : commands)
	{
		updateState(command->state);

		command->shader->bind();

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

	//TODO _ _ This render skybox is uncommented>
	this->clear(RenderBufferFlag::Depth);
	this->clear(RenderBufferFlag::Color);


	Layer * _layer = new Layer();

	Shader * layerShader = AssetManager::GetInstance().get<Shader>("phong_layer");
	_cache.scene->unloadUniforms(_layer->getUniforms());
	_layer->setQuad({ -1,-1 }, { 2, 2 }); //fullscreen
	_layer->setShader(layerShader);
	_layer->getInputs().clear();
	_layer->getInputs().insert(_layer->getInputs().begin(), pass.inputs.begin(), pass.inputs.end());
	_layer->setTarget(pass.target);
	_layer->render(this);

	delete _layer;


}
void  Renderer::renderForwardPass(const RenderPassCommand & pass)
{
	//pass the inputs depth buffer to the targets
	RenderTarget * input = pass.inputs[0];

	input->unloadAttachment(pass.target, RenderTargetAttachment::Depth, RenderTargetAttachment::Depth, 0, 0, _width, _height);

	renderCommands(pass.commands);

}


void  Renderer::renderScenePass(const RenderPassCommand& pass)
{
	//Rendering the scene doe nothing but load the Scene attachment to default framebuffers color attachment 0
	this->clear(RenderBufferFlag::Depth);
	this->clear(RenderBufferFlag::Color);

	float sampleRate = pass.targetScalar;
	RenderTarget* input = pass.inputs[0];
	
	int x = 0, y = 0;
	int width = _width / pass.targetScalar, height  = _height / pass.targetScalar;
	
	
	RenderTargetAttachment src = RenderTargetAttachment::Scene;
	RenderTargetAttachment dest = RenderTargetAttachment::Scene; //this is default attachment 0
	//copy color buffer target
	if(input)
	input->unloadAttachment(pass.target, src, dest, x, y, width, height);

	
	}

} 