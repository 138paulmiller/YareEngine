
#pragma once
#include <glm/glm.hpp>
#include "UniformBlock.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
/*

	Primitives are abstracted vertex arrays. There are not meshes as they do not store shader
*/

//Vertex must have position ,normal and uv attributes
namespace yare {
	namespace graphics {

		class Light {
		public:

			Light() { _camera = 0; }
			virtual ~Light() {
				delete _camera; _camera= 0;
			}
			//Lights are placed into an array. So, index must be specified
			virtual void unloadUniforms(UniformBlock& uniforms, int lightIndex) const= 0;
			virtual void setcastShadow(bool castsShadow) { _castsShadow = castsShadow;};
			virtual bool getCastShadow() const { return _castsShadow; }

			virtual void setShadowMap(Texture* shadowMap) { _shadowMap = shadowMap; };
			virtual Texture* getShadowMap() const { return _shadowMap; }
	

			Camera * getCamera() const { return _camera; }
			void setCamera(Camera * camera) { _camera = camera; }


		private:
			bool _castsShadow = true;
			Texture* _shadowMap = 0;
			Camera * _camera;
		};

		class PointLight : public Light
		{
		public:

			PointLight() {
				setCamera(new PerspectiveCamera(90, 1.0));//should be ortho camera
			};
			virtual ~PointLight() = default;
			void unloadUniforms(UniformBlock& uniforms, int lightIndex) const override;
			void setPosition   (const glm::vec3 &position );
			void setAmbient    (const glm::vec3 &ambient  );
			void setDiffuse    (const glm::vec3 &diffuse  );
			void setSpecular   (const glm::vec3 &specular );
			void setAttenuation(const glm::vec3& attenuation);

			const glm::vec3  & getPosition()   {return _position;}
			const glm::vec3  & getAmbient()	   {return _ambient;}
			const glm::vec3  & getDiffuse()	   {return _diffuse;}
			const glm::vec3  & getSpecular()   {return _specular;}
			const glm::vec3  & getAttenuation(){return _attenuation;}


			//defines its sphere of influence, will not cast shadows beyond this radius
			void setRadius(float radius);
		private:
			float _radius;
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
			DirectionalLight() {
				setCamera (new OrthographicCamera());//should be ortho camera
			};
			virtual ~DirectionalLight() = default;
			void unloadUniforms(UniformBlock& uniforms, int lightIndex)const override;

			void setDirection(glm::vec3& direction);
			void setPosition(glm::vec3& position);
			void setAmbient(glm::vec3& ambient);
			void setDiffuse(glm::vec3& diffuse);
			void setSpecular(glm::vec3& specular);
			

			const glm::vec3  & getPosition() { return _position; }
			const glm::vec3  & getAmbient() { return _ambient; }
			const glm::vec3  & getDiffuse() { return _diffuse; }
			const glm::vec3  & getSpecular() { return _specular; }
			const glm::vec3  & getDirection() { return _direction; }



		private:
			glm::vec3 _position;

			glm::vec3 _direction;
			glm::vec3 _ambient;
			glm::vec3 _diffuse;
			glm::vec3 _specular;
		};



	}
}