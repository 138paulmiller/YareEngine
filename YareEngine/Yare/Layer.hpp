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
		void setTarget(RenderTarget * target);
		void setQuad(const glm::vec2 & pos, const glm::vec2 & size);
		void render(Renderer * renderer);

		RenderTarget * getTarget() {
			return _target;
		}

	private:
		Shader * _shader;
		RenderTarget * _target; //what the effect renders to
		std::unique_ptr<VertexArray>  _quad; //Screen space quad

	};

}