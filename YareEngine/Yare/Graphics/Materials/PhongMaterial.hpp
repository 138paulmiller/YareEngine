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
			inline const Texture* getDiffuseTexture()  { return _diffuseTexture ; }
			inline const Texture* getSpecularTexture() { return _specularTexture; }
			inline const Texture* getEmissionTexture() { return _emissiveTexture; }
			inline float			getShininess()     { return _shininess; }


			//Create a switch for using either map or float3
			inline void setDiffuseTexture (Texture* texture) { _diffuseTexture  = texture; }
			inline void setSpecularTexture(Texture* texture) { _specularTexture = texture; }
			inline void setEmissiveTexture(Texture* texture) { _emissiveTexture = texture; }
			inline void setShininess(float shininess) { _shininess = shininess; }

		private:
			//All non-owning textures. Only maintains ref
			//Create a switch for using either map or float3
			Texture	* _diffuseTexture;
			Texture	* _specularTexture;
			Texture	* _emissiveTexture;
			float	  _shininess;
			
		};
	}
}