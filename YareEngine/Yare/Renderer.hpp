#pragma once
#include <stack>
#include <queue>

#include "Graphics/Camera.hpp"
#include "Graphics/Texture.hpp"
#include "Renderable.hpp"
#include "Scene.hpp"
#include "Graphics/LightBlock.hpp"

namespace yare {



	enum class RenderAPI
	{
		None = 0, OpenGL
	};



	//Runs on its own thread 
	//all render commands should be queued
	class Renderer
	{

	public:

		static Renderer* Create(RenderAPI api);
		//static Renderer *GetInstance();


		virtual ~Renderer() = default;
		
		//Used by the Scene Rendering 
		void beginScene(const Scene * scene);
		void submit(Renderable * renderable);
		void endScene();


		void present();

	protected:
		virtual void renderIndexedMesh(const graphics::VertexArray * vertexArray) = 0;
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
		const Scene * _scene; //current scene reference
	};
}
