 #pragma once
#include "Shader.hpp"
#include <unordered_map>

namespace yare { 
	namespace graphics {  
		enum class UniformType
		{
			Int, Float, Float3, Int3, Mat4
		};

		//Move to shader.hpp and use UBO
		struct Uniform
		{
			std::string name;
			union
			{
				int i;
				float f;
				glm::vec3 float3;
				glm::dvec3 int3;
				glm::mat4 mat4;
			}
			value;
			UniformType type;
		};

		//Create Uniform Buffer Layout Struct. Use this and just create a single templated set(Struct * struct)
		//https://www.lighthouse3d.com/tutorials/glsl-tutorial/uniform-blocks/
		class UniformBlock
		{
		public:
		
			void setUniform(const std::string& name, int i);
			void setUniform(const std::string& name, float f);
			void setUniform(const std::string& name, const glm::dvec3& int3);
			void setUniform(const std::string& name, const glm::vec3& float3);
			void setUniform(const std::string& name, const glm::mat4& mat4);
			
			/*
			Load all uniforms into shader
			*/
			void load(Shader* shader);
		private:
			std::unordered_map<std::string , Uniform> _uniforms;
		};

	} 
} 