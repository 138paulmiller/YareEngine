#pragma once
#include <stack>
#include <queue>

#include "Scene.hpp"
#include "Layer.hpp"
#include "Renderable.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/LightBlock.hpp"

namespace yare {

	class Renderer
	{

	public:

		static Renderer* Create(RenderAPI api);
		//static Renderer *GetInstance();

		virtual ~Renderer() = default;

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

		/*
			cached config state. render command only updates if different
		*/
	private:
		//The varying render passes
		void renderGeometry(const std::vector<RenderCommand * > & commands);
		//Each pass will read from this queue

		std::vector<RenderCommand * > _commands;
		std::stack<RenderState> _stateStack;
		Layer * _layer;

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
