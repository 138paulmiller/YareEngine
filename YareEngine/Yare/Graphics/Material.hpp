
#pragma once
#include <glm/glm.hpp>
#include "TextureBlock.hpp"
#include "UniformBlock.hpp"

namespace yare { namespace graphics  {

	class Material
	{
	public:
		virtual ~Material() = default;
		//Load uniform/textures into uniforms/texture block
		virtual void loadUniforms(UniformBlock & uniforms) = 0;
		virtual void loadTextures(TextureBlock & textures) = 0;
	};
}
}