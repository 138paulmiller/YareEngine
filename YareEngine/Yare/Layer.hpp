#pragma once
#include "Graphics/Mesh.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/RenderTarget.hpp"
/*
	Each Layer is a screenspace effect that accepts another layer. If none is provided default to screen. 
	
*/
namespace yare 
{
	using namespace graphics;
	class Renderer;
	//Layer should be a renderable !
	class Layer 
	{
	public:
		Layer();
		~Layer();

		void setShader(Shader * shader);
		void clearInputs();
		void addInput(RenderTarget * target);
		
		void setTarget(RenderTarget * target);
		void setQuad(const glm::vec2 & pos, const glm::vec2 & size);
		void render(Renderer * renderer);

		UniformBlock & getUniforms(){
			return _uniforms;
		}
		RenderTarget * getTarget() {
			return _target;
		}
		std::vector<RenderTarget *>  &  getInputs(){
			return _inputs;
		}
	private:
		Shader * _shader;
		//layer can also have input targets, just bind all the textures for input
		std::vector<RenderTarget *> _inputs; //textures to read from
	
		RenderTarget * _target; //what the effect renders to
		std::unique_ptr<VertexArray>  _quad; //Screen space quad
		UniformBlock _uniforms; //all available uniforms
	};

}