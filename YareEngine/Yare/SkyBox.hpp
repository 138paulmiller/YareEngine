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
	class SkyBox : public Mesh
	{
	public:
		//Add options for atmospheric effects
		SkyBox(int radius = 10, int sectors=30);
		~SkyBox();
		void preRender() override;
		void postRender() override;

		

	private:
		std::shared_ptr<Shader > _shader;
		std::shared_ptr<Texture> _texture;

	};
}
