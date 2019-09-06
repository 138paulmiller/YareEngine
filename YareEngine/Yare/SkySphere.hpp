#pragma once

#include <string>
#include <memory>
#include <Yare/Graphics/VertexArray.hpp>
#include <Yare/Graphics/Primitives.hpp>
#include <Yare/Graphics/Texture.hpp>
#include <Yare/System/FileSystem.hpp>


namespace yare
{
	class SkySphere
	{
	public:
		//Add options for atmospheric effects
		SkySphere(int radius = 10, int sectors=30);
		~SkySphere();
		void render(const glm::mat4& projection, const glm::mat4& view);

		glm::mat4& getModel();
		
		void setModel(glm::mat4& model);
		

	private:
		std::unique_ptr<graphics::SphereMesh>  _sphereMesh;
		std::unique_ptr<graphics::Shader > _shader;
		std::unique_ptr<graphics::Texture> _texture;
	};
}
