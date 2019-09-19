
#pragma once
#include <glm/glm.hpp>
#include "Renderer.hpp"
/*

	Primitives are abstracted vertex arrays. There are not meshes as they do not store shader
*/

//Vertex must have position ,normal and uv attributes
namespace yare {
	namespace graphics {

		class Light {
		public:
			virtual void loadUniforms(UniformBlock& uniforms) = 0;
		};


		class DirectionalLight : public Light
		{
			glm::vec3 direction;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		};


	}
}