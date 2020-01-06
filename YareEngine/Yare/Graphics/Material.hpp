
#pragma once
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "TextureBlock.hpp"
#include "UniformBlock.hpp"
#include "Renderable.hpp"

namespace yare { namespace graphics  {

	class Material
	{
	public:
		virtual ~Material() = default;
		//Load uniform/textures into uniforms/texture block
		virtual void unloadUniforms(UniformBlock & uniforms) = 0;
		virtual void unloadTextures(TextureBlock & textures) = 0;
		RenderCommand & getCommand() { return _command; }
	private:
		RenderCommand _command;
	};
}
}