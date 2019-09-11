#include "Shader.hpp"
#include <unordered_map>

YARE_GRAPHICS_MODULE_BEG
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
struct UniformBlock
{
	std::unordered_map<std::string , Uniform> uniforms;
	~UniformBlock()
	{
	}
	void setUniform(const std::string& name, int i)
	{
		Uniform & uniform = uniforms[name];
		uniform.name = name;
		uniform.value.i = i;
		uniform.type = UniformType::Int;
	}
	void setUniform(const std::string& name, float f)
	{

		Uniform& uniform = uniforms[name];
		uniform.name = name;
		uniform.value.f = f;
		uniform.type = UniformType::Float;

	}
	void setUniform(const std::string& name, const glm::dvec3& int3)
	{
		Uniform& uniform = uniforms[name];
		uniform.name = name;
		uniform.value.int3 = int3;
		uniform.type = UniformType::Int3;

	}
	void setUniform(const std::string& name, const glm::vec3& float3)
	{
		Uniform& uniform = uniforms[name];
		uniform.name = name;
		uniform.value.float3 = float3;
		uniform.type = UniformType::Float3;

	}

	void setUniform(const std::string& name, const glm::mat4& mat4)
	{
		Uniform& uniform = uniforms[name];
		uniform.name = name;
		uniform.value.mat4 = mat4;
		uniform.type = UniformType::Mat4;
	}


	void load(Shader* shader)
	{
		for (std::pair<std::string , Uniform > pair : uniforms)
		{

			Uniform & uniform = pair.second;
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
			}
		}
	}
};

YARE_GRAPHICS_MODULE_END