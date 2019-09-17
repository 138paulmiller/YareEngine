#pragma once 
#include "../Material.hpp"

namespace yare {
	namespace graphics {

		class PhongMaterial : public Material
		{
		public:
			~PhongMaterial() ;
			//Load uniform/textures into uniforms/texture block
			void loadUniforms(UniformBlock& uniforms) override ;
			void loadTextures(TextureBlock& textures) override ;

			//Create a switch for using either map or float3
			inline const Texture* getDiffuseTexture()  { return _diffuseTexture.get(); }
			inline const Texture* getSpecularTexture() { return _specularTexture.get(); }
			inline const Texture* getEmissionTexture() { return _emissiveTexture.get(); }
			inline float			getShininess()     { return _shininess; }


			//Create a switch for using either map or float3
			inline void setDiffuseTexture (Texture* texture) { _diffuseTexture. reset(texture); }
			inline void setSpecularTexture(Texture* texture) { _specularTexture.reset(texture); }
			inline void setEmissiveTexture(Texture* texture) { _emissiveTexture.reset(texture); }
			inline void setShininess(float shininess) { _shininess = shininess; }

		private:
			//Create a switch for using either map or float3
			std::shared_ptr<Texture> _diffuseTexture;
			std::shared_ptr<Texture> _specularTexture;
			std::shared_ptr<Texture> _emissiveTexture;
			float					 _shininess;
			
		};
	}
}