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
		SkyBox();
		~SkyBox();
		void preRender() override;
		void postRender() override;

		void setCubemap(Texture* cubemap);
		

		void loadFace(const TexturePixels& pixels, const TextureFace & face);

	private:
		std::shared_ptr<Shader > _shader;
		//Cubemap texture
		std::shared_ptr<Texture> _cubemap;

	};
}
