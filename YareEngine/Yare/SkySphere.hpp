#pragma once

#include <string>
#include <memory>
#include <Yare/Graphics/Renderer.hpp>
#include <Yare/Graphics/Primitives.hpp>
#include <Yare/Graphics/Texture.hpp>
#include <Yare/System/FileSystem.hpp>


namespace yare
{
	class SkySphere : public graphics::Renderable
	{
	public:
		//Add options for atmospheric effects
		SkySphere(int radius = 10, int sectors=30);
		~SkySphere();
		void render(const Renderer * renderer);

		glm::mat4& getModel();
		
		void setModel(glm::mat4& model);
		

	private:
		std::unique_ptr<graphics::SphereMesh>  _sphereMesh;
		std::shared_ptr<graphics::Shader > _shader;
		std::shared_ptr<graphics::Texture> _texture;
	};
}
