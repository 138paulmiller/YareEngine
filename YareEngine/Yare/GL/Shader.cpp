
#include "Shader.hpp"

#include "Error.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <fstream>

#include <iostream>
#include <stdexcept>
#include <vector>

using namespace glm;


GLuint compileShaderStage(const std::string& source, GLenum type) {
  // file loading
	
  // creation
	GLuint program = glCreateShader(type);
	if (program == 0)
		throw std::runtime_error("[Error] Impossible to create a new Shader");

	const char* sourceArr = &source[0] ;
  // code source assignation
  glShaderSource(program, 1, (const GLchar**)&sourceArr, NULL);

  // compilation
  glCompileShader(program);
  glCheckError();
  // compilation check
  GLint compile_status;
  glGetShaderiv(program, GL_COMPILE_STATUS, &compile_status);
  if (compile_status != GL_TRUE) {
    GLsizei logsize = 0;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logsize);

    char* log = new char[logsize + sizeof(char)];
    glGetShaderInfoLog(program, logsize, &logsize, log);

	std::cout << "[Error] compilation error: " << std::endl;
	std::cout << log << std::endl;

	program = 0;
	delete [] log;

  } else {
  }
  return program;
}


ShaderProgram::ShaderProgram()
	:shaderStages{0}  {
  program = 0;
}
ShaderProgram::~ShaderProgram() {
	glDeleteProgram(program);
}

void ShaderProgram::compile(const std::string& vertSource, const std::string& fragSource)
{
	if (program != 0) {
		glDeleteProgram(program);
	}
	program = glCreateProgram();
	if (!program)
		throw std::runtime_error("Impossible to create a new shader program");

	shaderStages[VERTEX_SHADER] = compileShaderStage(vertSource, GL_VERTEX_SHADER);
	shaderStages[FRAGMENT_SHADER] = compileShaderStage(fragSource, GL_FRAGMENT_SHADER);
    
	for (int i = 0; i < SHADER_STAGE_COUNT; i++) {
	
		glAttachShader(program, shaderStages[i]);
	}

	//link the compile shader stages
	glLinkProgram(program);
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		std::cout << "[Error] linkage error" << std::endl;

		GLsizei logsize = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize);

		char* log = new char[logsize];
		glGetProgramInfoLog(program, logsize, &logsize, log);

		std::cout << log << std::endl;
	}
	glCheckError();
}

GLint ShaderProgram::getUniform(const std::string& name) {
  auto it = uniforms.find(name);
  if (it == uniforms.end()) {
    // uniform that is not referenced
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc == GL_INVALID_OPERATION || loc < 0)
		std::cout << "[Error] uniform " << name << " doesn't exist in program" << std::endl;
    // add it anyways
    uniforms[name] = loc;

    return loc;
  } 
  return it->second;
}

GLint ShaderProgram::getAttribute(const std::string& name) {
  GLint attrib = glGetAttribLocation(program, name.c_str());
  if (attrib == GL_INVALID_OPERATION || attrib < 0)
	  std::cout << "[Error] Attribute " << name << " doesn't exist in program" << std::endl;

  return attrib;
}

void ShaderProgram::setAttribute(const std::string& name,
                                 GLint size,
                                 GLsizei stride,
                                 GLuint offset,
                                 GLboolean normalize,
                                 GLenum type) {
  GLint loc = getAttribute(name);
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, size, type, normalize, stride,
                        reinterpret_cast<void*>(offset));
}

void ShaderProgram::setAttribute(const std::string& name,
                                 GLint size,
                                 GLsizei stride,
                                 GLuint offset,
                                 GLboolean normalize) {
  setAttribute(name, size, stride, offset, normalize, GL_FLOAT);
}

void ShaderProgram::setAttribute(const std::string& name,
                                 GLint size,
                                 GLsizei stride,
                                 GLuint offset,
                                 GLenum type) {
  setAttribute(name, size, stride, offset, false, type);
}

void ShaderProgram::setAttribute(const std::string& name,
                                 GLint size,
                                 GLsizei stride,
                                 GLuint offset) {
  setAttribute(name, size, stride, offset, false, GL_FLOAT);
}

void ShaderProgram::setUniform(const std::string& name,
                               float x,
                               float y,
                               float z) {
  glUniform3f(getUniform(name), x, y, z);
}

void ShaderProgram::setUniform(const std::string& name, const vec3& v) {
  glUniform3fv(getUniform(name), 1, value_ptr(v));
}

void ShaderProgram::setUniform(const std::string& name, const dvec3& v) {
  glUniform3dv(getUniform(name), 1, value_ptr(v));
}

void ShaderProgram::setUniform(const std::string& name, const vec4& v) {
  glUniform4fv(getUniform(name), 1, value_ptr(v));
}

void ShaderProgram::setUniform(const std::string& name, const dvec4& v) {
  glUniform4dv(getUniform(name), 1, value_ptr(v));
}

void ShaderProgram::setUniform(const std::string& name, const dmat4& m) {
  glUniformMatrix4dv(getUniform(name), 1, GL_FALSE, value_ptr(m));
}

void ShaderProgram::setUniform(const std::string& name, const mat4& m) {
  glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, value_ptr(m));
}

void ShaderProgram::setUniform(const std::string& name, const mat3& m) {
  glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, value_ptr(m));
}

void ShaderProgram::setUniform(const std::string& name, float val) {
  glUniform1f(getUniform(name), val);
}

void ShaderProgram::setUniform(const std::string& name, int val) {
  glUniform1i(getUniform(name), val);
}


void ShaderProgram::use() const {
  glUseProgram(program);
}
void ShaderProgram::unuse() const {
  glUseProgram(0);
}

