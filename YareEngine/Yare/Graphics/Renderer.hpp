#pragma once
#include <stack>
#include <queue>

#include "Error.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "TextureBlock.hpp"
#include "UniformBlock.hpp"




#define DEFAULT_RENDER_API RenderAPI::OpenGL
#define TEXTURE_MAX 8
//https://www.gamedev.net/forums/topic/645442-game-engine-layout/
//https://www.gamedev.net/forums/topic/645295-should-game-objects-render-themselves-or-should-an-object-manager-render-them/
namespace yare {
	namespace graphics {

		


	// End Api

	enum class RenderAPI
	{
		None = 0, OpenGL
	};


	//Test functions to stencil, depth, test etc....
	enum class RenderTestFunc
	{
		Less = 0,
		LessEqual,
		Greater,
		GreaterEqual,
		Equal,
		NotEqual,
		Never, //Always fails.
		Always //Always passes
	};

	enum class RenderWinding {
		Clockwise = 0, CounterClockwise
	};

	enum class RenderCullFace {
		Back = 0, Front
	};

	enum class RenderPrimitive {
		Triangles = 0
	};


	enum class RenderMode {
		IndexedMesh = 0
	};

	//overload == to only update if changed 
	struct RenderState
	{
		RenderCullFace cullFace = RenderCullFace::Back;
		RenderWinding  winding = RenderWinding::Clockwise;
		RenderTestFunc depthFunc = RenderTestFunc::Less;
		RenderTestFunc stencilFunc = RenderTestFunc::Less;
		bool wireframe = 0;
	};


	//Should order render data.
	struct RenderData
	{
		//Render Targets 
		//Viewport
		UniformBlock uniforms;
		TextureBlock textures;

		VertexArray * vertexArray = 0;
		Shader * shader = 0;

		RenderMode mode = RenderMode::IndexedMesh;
		RenderPrimitive primitive = RenderPrimitive::Triangles;
		RenderState state;
	};

	
	struct Renderable
	{
		virtual ~Renderable() = default;
		virtual void preRender() = 0;
		virtual void postRender() = 0;
		RenderData renderData;
	};


	//Runs on its own thread 
	//all render commands should be queued
	class Renderer
	{
	public:

		static Renderer* Create(RenderAPI api);
		static Renderer *GetInstance();

		virtual ~Renderer() = default;
		void submit(Renderable * renderable);
		void present();
		void beginScene(const Camera * camera);
		void endScene();
		inline RenderAPI getAPI() { return _api; }

	protected:
		virtual void renderIndexedMesh(const VertexArray * vertexArray) = 0;
		virtual void updateState(const RenderState & state) = 0;
		
		/*
			cached config state. render command only updates if different
		*/
	private:
		std::queue<Renderable * > _renderQueue;
		std::stack<RenderState> _stateStack;


		//current environment map. Rendered to in deferred pass. and used for reflection/refractions in transparency 
		//Or, create CaptureCube. Renders all item within the region to a cubemap. This can then be bound to the environment map
		//SkyBox _skybox; 
		const Camera * _camera; //current 
		RenderAPI _api;
	};



	} 
}
