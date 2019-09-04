
#pragma once

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <initializer_list>
#include <map>
#include <string>


// Shader Interface
class Shader {
 public:

	 static Shader* Create();

	 virtual ~Shader() = default;

	//Destroy program
	virtual void destroy() = 0;

	virtual void compile(const std::string& vertSource, const std::string& fragSource) = 0;

	// bind the program
	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void setUniform(const std::string& name, const glm::vec3& v)  = 0;
	virtual void setUniform(const std::string& name, const glm::dvec3& v) = 0;
	virtual void setUniform(const std::string& name, const glm::vec4& v)  = 0;
	virtual void setUniform(const std::string& name, const glm::dvec4& v) = 0;
	virtual void setUniform(const std::string& name, const glm::dmat4& m) = 0;
	virtual void setUniform(const std::string& name, const glm::mat4& m)  = 0;
	virtual void setUniform(const std::string& name, const glm::mat3& m)  = 0;
	virtual void setUniform(const std::string& name, float val) =0;
	virtual void setUniform(const std::string& name, int val)   =0;


};
