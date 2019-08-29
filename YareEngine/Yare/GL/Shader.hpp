
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
class Shader {
 public:
	 enum Stage {
		 
		FRAGMENT_SHADER = 0,
		VERTEX_SHADER ,
		SHADER_STAGE_COUNT
	};

	// constructor
	 Shader();

	~Shader();

	//Destroy program
	void destroy();

	void compile(const std::string& vertSource, const std::string& fragSource);

	// bind the program
	void bind() const;
	void unbind() const;


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


protected:

	void cacheAttributeAndUniforms();
	GLuint compileStage(const std::string& source, GLenum type);

private:

	//name to location Cache to avoid querying drivers for program info
	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> attributes;
	
	//handles to attached shader stages
	GLuint stages[SHADER_STAGE_COUNT];
	// opengl id
	GLuint program;

	bool isValid;

};
