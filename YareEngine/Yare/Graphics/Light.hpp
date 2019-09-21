
#pragma once
#include <glm/glm.hpp>
#include "UniformBlock.hpp"
/*

	Primitives are abstracted vertex arrays. There are not meshes as they do not store shader
*/

//Vertex must have position ,normal and uv attributes
namespace yare {
	namespace graphics {

		class Light {
		public:

			virtual ~Light() = default;
			//Lights are placed into an array. So, index must be specified
			virtual void loadUniforms(UniformBlock& uniforms, int lightIndex) const= 0;
		};

		class PointLight : public Light
		{
		public:

			virtual ~PointLight() = default;
			void loadUniforms(UniformBlock& uniforms, int lightIndex) const override;

			void setPosition   (glm::vec3 &position );
			void setAmbient    (glm::vec3 &ambient  );
			void setDiffuse    (glm::vec3 &diffuse  );
			void setSpecular   (glm::vec3 &specular );
			void setAttenuation(glm::vec3& attenuation);
			//Cutoff for spotlight effect
			void setCutOff(float inner, float outer);
		private:
			float _innerCutOff;
			float _outerCutOff;
			glm::vec3 _position;
			glm::vec3 _ambient;
			glm::vec3 _diffuse;
			glm::vec3 _specular;
			//attenuation coefficients {constan, linear, quadratic}
			glm::vec3 _attenuation;
		};

		class DirectionalLight : public Light
		{
		public:
			virtual ~DirectionalLight() = default;
			void loadUniforms(UniformBlock& uniforms, int lightIndex)const override;
			void setDirection(glm::vec3& position);
			void setAmbient(glm::vec3& ambient);
			void setDiffuse(glm::vec3& diffuse);
			void setSpecular(glm::vec3& specular);
			
		private:
			glm::vec3 _direction;
			glm::vec3 _ambient;
			glm::vec3 _diffuse;
			glm::vec3 _specular;
		};



	}
}