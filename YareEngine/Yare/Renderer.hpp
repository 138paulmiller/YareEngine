#pragma once
#include <stack>
#include <queue>
#include <functional>
#include "Scene.hpp"
#include "Layer.hpp"
#include "Renderable.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/LightBlock.hpp"

namespace yare {



	enum class RenderAPI
	{
		None = 0, OpenGL
	};


	/*
		TODO - Sort RenderCommands using bucketing approach. 
		Sort by the layer its rendering to, shader, texture etc...
	
		//see https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
	*/
	enum class RenderBufferFlag : char
	{
		Color = 0b000001, 
		Depth = 0b000010,
	};

	enum class RenderPass
	{
		//renders position, color, normal to rendertarget geometry. all layers can access this
		Geometry = 0,
		//Uses Gbuffer data to write lit scene info to a buffer
		Lighting,
		//SSAO,
		Forward,
		Count 
	};

	//Create a callback for pre and post pass hooks. The cretae classes for each pass type, will manage drawing its own layer and clearing
	struct RenderPassCommand
	{
		//Each Pass
		std::vector<RenderCommand * > commands;
		RenderTarget * target;
		//layer can also have input targets, just bind all the textures for input
		std::vector<RenderTarget *> inputs;
		
		void (Renderer::*render)(const RenderPassCommand & passCommand);
		//0-1 scalar that will scale the resolution of the target. low value higher perf
		float sampleRate = 1.0;
	};

	//Runs on its own thread 
	//all render commands should be queued
	class Renderer
	{

	public:

		static Renderer* Create(RenderAPI api);
		//static Renderer *GetInstance();

		Renderer() ;
		virtual ~Renderer() ;

		virtual void clear(RenderBufferFlag mask) = 0;
		virtual void renderMesh(const VertexArray* vertexArray) = 0 ;
		virtual void renderIndexedMesh(const graphics::VertexArray * vertexArray) = 0;
		
		virtual void resizeViewport(int width, int height);
		//Used by the Scene Rendering . If target is null, forward renders to screen 
		void begin(Scene * scene);
		void submit(Renderable * renderable);
		void end();

		virtual void render();

	
	protected:
		virtual void updateState(const RenderState & state) = 0;
		void renderCommands(const std::vector<RenderCommand * > & commands);

		/*
			cached config state. render command only updates if different
		*/
	private:
		//The varying render passes
		//returns target that was rendered to
		void setupRenderPasses();
		void renderPass(RenderPass pass);
		void renderGeometryPass(const RenderPassCommand & pass);
		void renderLightingPass(const RenderPassCommand & pass);
		void renderForwardPass(const RenderPassCommand & pass);
		
		
		void renderColor();
		
		RenderPassCommand _passes[(const int)RenderPass::Count];
		//Each pass will read from this queue
		using RenderBuffers = std::vector<RenderTarget *>;
		RenderBuffers _targets;

		std::stack<RenderState> _stateStack;


		//current environment map. Rendered to in deferred pass. and used for reflection/refractions in transparency 
		//Or, create CaptureCube. Renders all item within the region to a cubemap. This can then be bound to the environment map
		//SkyBox _skybox; 
		struct RenderCache
		{
			//Cached for begin/end blocks
			Scene * scene; //current scene 
		} _cache;

		//todo : create viewport  class
		int _width, _height;
	};
}
