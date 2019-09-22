#pragma once
#include "Shader.hpp"
#include "Light.hpp"

#include <unordered_map>


//Taken from glsl code. should append based on config value
#define POINT_LIGHT_COUNT 64
#define DIRECTIONAL_LIGHT_COUNT 64

namespace yare {
	namespace graphics {
		

		//Create Uniform Buffer Layout Struct. Use this and just create a single templated set(Struct * struct)
		//https://www.lighthouse3d.com/tutorials/glsl-tutorial/uniform-blocks/
		struct LightBlock
		{
		public:
			void setPointLight(const std::string & name, PointLight* light);
			void setDirectionalLight(const std::string& name, DirectionalLight* light);
			
			void removePointLight(const std::string& name);
			void removeDirectionalLight(const std::string& name );


			/*
			Load all lights into shader
			*/
			void loadUniforms(UniformBlock & uniforms) const;
		private:
			std::unordered_map<std::string, PointLight * > _pointLights;
			std::unordered_map<std::string, DirectionalLight * > _directionalLights;
		};

	}
}