#pragma once
#include "Graphics/Camera.hpp"
#include "Graphics/LightBlock.hpp"
#include "Renderable.hpp"
#include "Graphics/UniformBlock.hpp"
#include "Graphics/RenderTarget.hpp"

namespace yare 
{ 
	using namespace graphics;
	class Renderer;

	//Does not garbage collect graphics resources. Only has weak pointers!
	class Scene
	{
	public:
		
		LightBlock  & getLights();

		void setCamera(const Camera * camera);
		const Camera* getCamera() const;
		//Loads all camera nd light data into uniform block
		void loadUniforms(UniformBlock& uniforms) const;

		void add(const std::string & name, Renderable * renderable) ;
		void remove(const std::string& name);
		

		void render(Renderer* renderer);
	private:
		const Camera* _camera; //current cameras scene
		LightBlock _lights; //all lights in the current scene
		std::unordered_map<std::string, Renderable* > _renderables;
	};
}
