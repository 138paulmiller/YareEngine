
#pragma once

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <initializer_list>
#include <map>
#include <string>


// A shader program is a set of shader (for instance vertex shader + pixel
// shader) defining the rendering pipeline.
//
// This class provide an interface to define the OpenGL uniforms and attributes
// using GLM objects.
class ShaderProgram {
 public:
	 enum ShaderStage {
		 
		FRAGMENT_SHADER = 0,
		VERTEX_SHADER ,
		SHADER_STAGE_COUNT
	};

	// constructor
	ShaderProgram();

	~ShaderProgram();

	void compile(const std::string& vertSource, const std::string& fragSource);

	// bind the program
	void use() const;
	void unuse() const;


	// clang-format off
	// provide attributes informations.
	GLint getAttribute(const std::string& name);
	void setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize, GLenum type);
	void setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize);
	void setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset, GLenum type); 
	void setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset);
	// clang-format on

	// provide uniform location
	GLint getUniform(const std::string& name);
 
	// affect uniform
	void setUniform(const std::string& name, float x, float y, float z);
	void setUniform(const std::string& name, const glm::vec3& v);
	void setUniform(const std::string& name, const glm::dvec3& v);
	void setUniform(const std::string& name, const glm::vec4& v);
	void setUniform(const std::string& name, const glm::dvec4& v);
	void setUniform(const std::string& name, const glm::dmat4& m);
	void setUniform(const std::string& name, const glm::mat4& m);
	void setUniform(const std::string& name, const glm::mat3& m);
	void setUniform(const std::string& name, float val);
	void setUniform(const std::string& name, int val);


private:

	//Cached names
	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> attributes;


	GLuint shaderStages[SHADER_STAGE_COUNT];
	// opengl id
	GLuint program;

};
