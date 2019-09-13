#pragma once

#include <string>
#include <memory>
#include "Graphics/Mesh.hpp"
#include "Graphics/Texture.hpp"
#include "OS/FileSystem.hpp"


namespace yare
{
	//Instead of using sphere mesh, just cretae the vao manually
	//To createa a sky sphere just create a sphere mesh with phong material. use spherical enviroment map and swithc front face
	using namespace graphics;
	class SkyBox : public Renderable
	{
	public:
		//Add options for atmospheric effects
		SkyBox(int radius = 10, int sectors=30);
		~SkyBox();
		void render(Renderer * renderer) override;

		glm::mat4& getModel();
		
		void setModel(glm::mat4& model);
		

	private:
		std::unique_ptr<Mesh>  _mesh;
		std::shared_ptr<Shader > _shader;
		std::shared_ptr<Texture> _texture;

	};
}
