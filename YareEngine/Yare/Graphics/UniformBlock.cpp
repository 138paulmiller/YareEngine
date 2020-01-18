#include "UniformBlock.hpp"


namespace yare {
	namespace graphics {

		//Create Uniform Buffer Layout Struct. Use this and just create a single templated set(Struct * struct)
		//https://www.lighthouse3d.com/tutorials/glsl-tutorial/uniform-blocks/
			void UniformBlock::setUniform(const std::string& name, int i)
			{
				Uniform& uniform = _uniforms[name];
				uniform.name = name;
				uniform.value.i = i;
				uniform.type = UniformType::Int;
			}
			void UniformBlock::setUniform(const std::string& name, float f)
			{

				Uniform& uniform = _uniforms[name];
				uniform.name = name;
				uniform.value.f = f;
				uniform.type = UniformType::Float;

			}
			void UniformBlock::setUniform(const std::string& name, const glm::dvec3& int3)
			{
				Uniform& uniform = _uniforms[name];
				uniform.name = name;
				uniform.value.int3 = int3;
				uniform.type = UniformType::Int3;

			}
			void UniformBlock::setUniform(const std::string& name, const glm::vec3& float3)
			{
				Uniform& uniform = _uniforms[name];
				uniform.name = name;
				uniform.value.float3 = float3;
				uniform.type = UniformType::Float3;

			}

			void UniformBlock::setUniform(const std::string& name, const glm::mat4& mat4)
			{
				Uniform& uniform = _uniforms[name];
				uniform.name = name;
				uniform.value.mat4 = mat4;
				uniform.type = UniformType::Mat4;
			}

			void UniformBlock::setUniform(const std::string& name, Texture* texture)
			{
				Uniform& uniform = _uniforms[name];
				uniform.name = name;
				uniform.value.texture= texture;
				uniform.type = UniformType::Texture;
			}

			void UniformBlock::load(Shader* shader) const
			{
				int unit = 0;
				for (std::pair<std::string, Uniform > pair : _uniforms)
				{

					Uniform& uniform = pair.second;
					switch (uniform.type)
					{
					case UniformType::Int:
						shader->setUniform(pair.first, uniform.value.i);
						break;
					case UniformType::Float:
						shader->setUniform(pair.first, uniform.value.f);
						break;
					case UniformType::Float3:
						shader->setUniform(pair.first, uniform.value.float3);
						break;
					case UniformType::Int3:
						shader->setUniform(pair.first, uniform.value.int3);
						break;
					case UniformType::Mat4:
						shader->setUniform(pair.first, uniform.value.mat4);
						break;
					case UniformType::Texture:
						Texture * texture = uniform.value.texture;
						const std::string &  name = pair.first;
						//bind texture to unit i
						if (texture) {
							texture->bind(unit);
							shader->setUniform(name, unit);
							//set sampler to this unit
							//next unit
							unit++;
						}
			
						break;
					}
				}
			}
	}
}