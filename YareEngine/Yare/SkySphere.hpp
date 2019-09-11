#pragma once

#include <string>
#include <memory>
#include "Graphics/Primitives.hpp"
#include "Graphics/Texture.hpp"
#include "System/FileSystem.hpp"


namespace yare
{
	using namespace graphics;
	class SkySphere : public Renderable
	{
	public:
		//Add options for atmospheric effects
		SkySphere(int radius = 10, int sectors=30);
		~SkySphere();
		void render(Renderer * renderer) override;

		glm::mat4& getModel();
		
		void setModel(glm::mat4& model);
		

	private:
		std::unique_ptr<SphereMesh>  _sphereMesh;
		std::shared_ptr<Shader > _shader;
		std::shared_ptr<Texture> _texture;
		RenderState _state;
	};
}
