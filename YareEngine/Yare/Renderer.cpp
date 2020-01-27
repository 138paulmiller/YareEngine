#include "Renderer.hpp"
#include "Graphics/OpenGL/OpenGLRenderer.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Light.hpp"


//Remove 
#include "AssetManager.hpp"
#include "Graphics/OpenGL/OpenGLError.hpp"
namespace yare { 


RenderBufferFlag operator|(RenderBufferFlag l, RenderBufferFlag r) { return RenderBufferFlag((int)l | (int)r); }
RenderBufferFlag operator&(RenderBufferFlag l, RenderBufferFlag r) { return RenderBufferFlag((int)l & (int)r); }


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
	_cache.camera = 0 ;
	_cache.lights = 0;
	setupLayers();
	setupTargets();
	setupRenderPasses();
}
Renderer::~Renderer()
{
	for (RenderTargetMap::iterator it = _targets.begin();it != _targets.end(); it++) {
		delete it->second;
		it->second;
	}
	for (LayerMap::iterator it = _layers.begin(); it != _layers.end(); it++) {
		delete it->second;
		it->second;
	}
	for (RenderTarget* target : _cache.shadowmapTargets)
	{
		delete target;
		target = 0;
	}
}

void Renderer::resizeViewport(int width, int height)
{
	_width = width;
	_height = height;
}

void Renderer::pushState(const RenderState & state )
{
	updateState(state);
	_stateStack.push(state);
}

void Renderer::popState( )
{
	_stateStack.pop();
	if (_stateStack.empty()) return;
	updateState(_stateStack.top());
}

void Renderer::begin(Scene* scene)
{
	_cache.camera = scene->getCamera();
	_cache.lights = &scene->getLights();


}
void Renderer::submit(Renderable * renderable)
{
	RenderCommand * command  = &renderable->command;
	
	switch (renderable->command.lighting)
	{
		case RenderLighting::Surface:
			//surface, add commands to 
			_passes[(int)RenderPass::Geometry].commands.push_back(command);

			break;
		case RenderLighting::Unlit:
			_passes[(int)RenderPass::Forward].commands.push_back(command);
			break;

	}
	

}
void Renderer::end()
{
	
	//cleanup

	for (int i = 0; i < (const int)RenderPass::Count; i++)
	{
		_passes[i].commands.clear();
	}
	_cache.camera = 0;
	_cache.lights = 0;
}


void Renderer::renderLayer(Layer * layer, const Camera* camera, const LightBlock* lights, const std::vector<RenderTarget*>& inputs, RenderTarget* target)
{

	camera->unloadUniforms(layer->getUniforms());
	lights->unloadUniforms(layer->getUniforms());
	layer->getInputs().clear();
	layer->getInputs().insert(layer->getInputs().begin(), inputs.begin(), inputs.end());
	layer->setTarget(target);
	layer->render(this);
	
}


void Renderer::renderCommands(const std::vector<RenderCommand* >& commands, const Camera* camera , const LightBlock* lights )
{
	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	for (RenderCommand* command : commands)
	{
		updateState(command->state);
		command->shader->bind();
		/*
		Instead of loading all uniforms and texture each frame. Create Shader Instances that are copies of a base shader.
		Will only have to bind when dirty. Since they are copies, they should not be affected by other renderables that share the same parent shader
		Potentially do this at the material level and have materials instances maintain *blocks.
		*/
		//get the uniforms from the scene
		if(camera)
			camera->unloadUniforms(command->uniforms);
		if(lights)
			lights->unloadUniforms(command->uniforms);
		//load the uniforms into the shader

		command->uniforms.load(command->shader);
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

	}
}


void Renderer::setupTargets()
{

	RenderTarget* gbuffer = RenderTarget::Create();
	gbuffer->setup({
		//must match glsl layout
		RenderTargetAttachment::Position,
		RenderTargetAttachment::Normal,
		RenderTargetAttachment::Depth,
		RenderTargetAttachment::Diffuse,
		RenderTargetAttachment::Specular,
		//should use depth

		}
	);

	//glsl must write to scene
	RenderTarget* scene = RenderTarget::Create();
	scene->setup({
		RenderTargetAttachment::Scene,
		RenderTargetAttachment::Depth,

		//should use depth
		}
	);

	_targets["gbuffer"] = gbuffer;
	_targets["scene"] = scene;

	//generate shadowmap targets
	int lightCount = Light::MAX_DIRECTIONAL_LIGHT_COUNT + Light::MAX_POINT_LIGHT_COUNT;
	//create as many shadowmaps as there are lights
	//should automatically create an array of render targets, equal to the number of maximum possible lights. prevents alloc/dealloc per frame 
	_cache.shadowmapTargets.clear();
	_cache.shadowmapTargets.resize(lightCount);
	for (int i = 0; i < lightCount; i++)
	{
		RenderTarget* target = RenderTarget::Create();
		target->setup({
			RenderTargetAttachment::Depth,
			}
		);
		_cache.shadowmapTargets[i] = target;
	}

}
void Renderer::setupLayers()
{
	Layer* phongLayer = new Layer();
	phongLayer->setShader(AssetManager::GetInstance().get<Shader>("phong_layer"));
	phongLayer->setQuad({ -1,-1 }, { 2, 2 }); //fullscreen
	_layers["phong"] = phongLayer;


}

void Renderer::setupRenderPasses()
{
	for (int i = 0; i < (const int)RenderPass::Count; i++) {
		_passes[i].target = 0;
	}

	//Setup geometry pass
	RenderPassCommand & geometryPass = _passes[(const int)RenderPass::Geometry];
	geometryPass.render = &Renderer::renderPassGeometry;
	geometryPass.targetScalar = 1.0;
	geometryPass.target = _targets["gbuffer"];

	float sceneBufferScalar = 1.0 ; 
	//float sceneBufferScalar = 1.0 / 5.0;



	//Setup lighting pass
	RenderPassCommand & lightingPass = _passes[(const int)RenderPass::Lighting];
	lightingPass.targetScalar = sceneBufferScalar;

	lightingPass.render = &Renderer::renderPassLighting;
	lightingPass.inputs = { _targets["gbuffer"] };//default framebuffer;
	lightingPass.target = _targets["scene"];
	
	//Setup forward pass
	RenderPassCommand & forwardPass = _passes[(const int)RenderPass::Forward];
	forwardPass.targetScalar = sceneBufferScalar;

	forwardPass.render = &Renderer::renderPassForward;
	forwardPass.inputs = { _targets["gbuffer"] };//uses gbuffers depth buffer 
	forwardPass.target = _targets["scene"];



	//Setup scene pass
	RenderPassCommand& scenePass = _passes[(const int)RenderPass::Scene];
	scenePass.targetScalar = sceneBufferScalar;

	scenePass.render = &Renderer::renderPassScene;
	scenePass.inputs = { _targets["scene"] };//uses gbuffers depth buffer 
	scenePass.target = 0;//uses gbuffers depth buffer 


}
//Enable forward rendering, for all unlit, render these to the screen unlit.
//the render deferred passes 
void Renderer::render()
{

	renderPass(RenderPass::Geometry);
	
	if (!_settings.debugShadowmap)
	{
		renderPass(RenderPass::Lighting);
		renderPass(RenderPass::Forward);
		renderPass(RenderPass::Scene);
	}
//render post processes, initial post process should use scene as input. chaiun outputs into inputs
	//as of now, only geometry passa and forward use commands. all other are just layer draws of buffer copies


	if (_settings.debugGBuffer)
	{
		debugRenderTarget(_targets["gbuffer"]);
	}
}


void Renderer::renderPass(RenderPass pass)
{
	RenderPassCommand& passCommand = _passes[(int)pass];
	//Resize target to match desired dimensions
	if (passCommand.target) {
		//bind the target to render to
		int w = _width;
		int h = _height;
		if (passCommand.target->getWidth() != w || passCommand.target->getHeight() != h) {
			passCommand.target->resize(w * passCommand.targetScalar, h * passCommand.targetScalar);
		}
		passCommand.target->bind();

		resizeViewport(passCommand.target->getWidth(), passCommand.target->getHeight());

	}

	//render
	if (passCommand.render)
		(this->* (passCommand.render))(passCommand);

	//unbind the rendered target
	if (passCommand.target)
		passCommand.target->unbind();

	//set back to default viewport size
	resizeViewport(_width, _height);
}

void Renderer::renderPassGeometry(const RenderPassCommand & pass)
{
	this->clear(RenderBufferFlag::Depth | RenderBufferFlag::Color);
	renderCommands(pass.commands, _cache.camera, _cache.lights);	
	//generate shadow maps
	generateShadowmaps(_cache.shadowmapTargets, pass.commands, _cache.lights);

}


void  Renderer::renderPassLighting(const RenderPassCommand & pass)
{
	//render the lit scene
	this->clear(RenderBufferFlag::Depth | RenderBufferFlag::Color);
	renderLayer(_layers["phong"], _cache.camera, _cache.lights,pass.inputs, pass.target);
	//cleanup
}


void  Renderer::renderPassForward(const RenderPassCommand & pass)
{

	//pass the inputs depth buffer to the targets
	RenderTarget * input = pass.inputs[0];
	//todo - write depth to texture, the render quad and wrtie to gl_FRragDepth
	input->blit(pass.target, RenderTargetAttachment::Depth, RenderTargetAttachment::Depth, 0, 0, _width, _height);
	renderCommands(pass.commands, _cache.camera, _cache.lights);

}


void  Renderer::renderPassScene(const RenderPassCommand& pass)
{
	//Rendering the scene doe nothing but load the Scene attachment to default framebuffers color attachment 0
	this->clear( RenderBufferFlag::Depth | RenderBufferFlag::Color);

	float sampleRate = pass.targetScalar;
	RenderTarget* input = pass.inputs[0];
	
	int x = 0, y = 0;
	int width = _width / pass.targetScalar, height  = _height / pass.targetScalar;
	
	
	RenderTargetAttachment src = RenderTargetAttachment::Scene;
	RenderTargetAttachment dest = RenderTargetAttachment::Scene; //this is default attachment 0
	//copy color buffer target
	if(input)
		input->blit(pass.target, src, dest, x, y, width, height);

	
}


void  Renderer::generateShadowmaps(std::vector<RenderTarget* >& targets, const std::vector<RenderCommand* >& commands, LightBlock * lights)
{
	Shader* lightDepthShader = (AssetManager::GetInstance().get<Shader>("light_depth"));
	LightBlock::Lights<PointLight*>        pointLights = lights->getPointLights();
	LightBlock::Lights<DirectionalLight*> directionalLights = lights->getDirectionalLights();
	int lightCount = pointLights.size() + directionalLights.size();
	//create as many shadowmaps as there are lights? 
	int index = 0;


	//for point lights, create an Env RenderTargetAttachment for cubemap support!!!

	//for each light, render the scene depth.
	for (LightBlock::Lights<DirectionalLight*>::value_type value : directionalLights) {
		DirectionalLight* dirLight = value.second;
		if (dirLight->getCastShadow()) {
			RenderTarget* target = targets[index];
			renderShadowmap(target , commands, dirLight, lightDepthShader);
			dirLight->setShadowMap(target->getTexture(RenderTargetAttachment::Depth));
			//debug shad
			if(_settings.debugShadowmap)
				target->blit(0, RenderTargetAttachment::Depth, RenderTargetAttachment::Scene, 0,0, _width, _height);
			index++;
		}
	}


}


void Renderer::renderShadowmap(RenderTarget* target, const std::vector<RenderCommand * >& commands, Light * light, Shader* shader)
{
	target->resize(_width, _height);
	target->bind(RenderTargetMode::Draw);
	this->clear(RenderBufferFlag::Depth | RenderBufferFlag::Color);
	float aspect = ((float)_width) / _height;
	light->getCamera()->setAspect(aspect);
	shader->bind();
	RenderState shadowState;
	shadowState.cullFace = RenderCullFace::Front;	//cull front face to prevent "peter-panning" effect
	//TODO - enable blending to accumulate value 
	updateState(shadowState);

	for (RenderCommand* command : commands)
	{
		const glm::mat4& lightSpace = light->getCamera()->getProjection() * light->getCamera()->getView();
		light->getCamera()->unloadUniforms(command->uniforms);
		command->uniforms.load(shader);

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
	}
	updateState(RenderState());
	target->unbind();

}

//////////////////////////////////////////////////////////////////////////////////////////

//unloads all attachments toy default framebuffer
void Renderer::debugRenderTarget( RenderTarget* target)
{
	if (target == 0) return;
	//this->clear(RenderBufferFlag::Depth | RenderBufferFlag::Color);

	std::vector<RenderTargetAttachment> attachments;
	target->getAttachments(attachments);
	int n = attachments.size(); //also render entire scene
	//divide the screen for each quad
	int square = floor(sqrt(n))+1;
	int x = 0, y = 0;
	int w = _width / square;
	int h = _height / square;

	RenderTargetAttachment dest = RenderTargetAttachment::Scene; //this is default color attachment 0
	//copy color buffer target
	for (RenderTargetAttachment attachment : attachments)
	{
		target->blit(0, attachment, dest, x, y, w, h);
		x += w;
		if (x >= _width) {
			x = 0;
			y += h;
		}
	}

}

} 

