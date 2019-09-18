#pragma once

#include <string>
#include <memory>
#include "Mesh.hpp"
#include "Texture.hpp"


namespace yare
{
	namespace graphics
	{
		//Instead of using sphere mesh, just cretae the vao manually
		//To createa a sky sphere just create a sphere mesh with phong material. use spherical enviroment map and swithc front face
		class SkyBox : public Mesh
		{
		public:
			//Add options for atmospheric effects
			SkyBox();
			~SkyBox();


			void setCubemap(Texture* cubemap);
			void loadFace(const TexturePixels& pixels, const TextureFace& face);

		private:
			//Non owning pointers 
			Shader * _shader;
			//Cubemap texture
			Texture * _cubemap;

		};
	}
}
