
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



Shader::Shader()
	:stages{0}  
{
  program = 0;
  isValid = false;
}

Shader::~Shader()
{
	destroy();
}

void Shader::destroy() 
{
	if (program == 0) return;
	isValid = false;
	attributes.clear();
	uniforms.clear();
	//Link the compiled stages
	for (int i = 0; i < SHADER_STAGE_COUNT; i++)
	{
		glDetachShader(program, stages[i]);
	}
	glDeleteProgram(program);
}
GLuint Shader::compileStage(const std::string& source, GLenum type)
{
	// creation
	GLuint stage = glCreateShader(type);
	if (stage == 0)
		throw std::runtime_error("[Error] Driver could not allocate new shader stage");

	const char* sourceArr = &source[0];
	// code source assignation
	glShaderSource(stage, 1, (const GLchar * *)& sourceArr, NULL);

	// compilation
	glCompileShader(stage);
	glCheckError();
	// compilation check
	GLint status;
	glGetShaderiv(stage, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		GLsizei logsize = 0;
		glGetShaderiv(stage, GL_INFO_LOG_LENGTH, &logsize);

		char* log = new char[logsize + sizeof(char)];
		glGetShaderInfoLog(stage, logsize, &logsize, log);

		std::cout << "[Error] Stage Compilation error: " << std::endl;
		std::cout << log << std::endl;

		stage = 0;
		delete[] log;
		return -1;

	}
	else {
	}
	return stage;
}



void Shader::compile(const std::string& vertSource, const std::string& fragSource)
{
	if (program != 0) {
		destroy();
	}
	program = glCreateProgram();
	if (!program)
		throw std::runtime_error("[Error] Driver could not allocate new shader stage");

	stages[VERTEX_SHADER] = compileStage(vertSource, GL_VERTEX_SHADER);
	stages[FRAGMENT_SHADER] = compileStage(fragSource, GL_FRAGMENT_SHADER);
    
	//Link the compiled stages
	for (int i = 0; i < SHADER_STAGE_COUNT; i++) 
	{
		glAttachShader(program, stages[i]);
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


	cacheAttributeAndUniforms();
}


void Shader::cacheAttributeAndUniforms()
{
	GLint i;
	GLint count;
	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)
	// maximum name length is 16
	GLchar name[16]; // variable name in GLSL
	GLsizei length; // name length
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
	
	for (int i = 0; i < count; i++)
	{
		glGetActiveAttrib(program, (GLuint)i, sizeof(name)/sizeof(GLchar), &length, &size, &type, name);
		//cache its location
		attributes[name] = glGetAttribLocation(program, name);
		printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
	}
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(program, (GLuint)i, sizeof(name) / sizeof(GLchar), &length, &size, &type, name);
		//cache its location
		uniforms[name] = glGetUniformLocation(program, name);
		printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
	}
}

GLint Shader::getUniform(const std::string& name) 
{
  auto it = uniforms.find(name);
  if (it == uniforms.end()) {
 	std::cout << "[Error] uniform " << name << " doesn't exist in program" << std::endl;
    return -1;

  } 
  return it->second;
}

GLint Shader::getAttribute(const std::string& name) 
{
	auto it = attributes.find(name);
	if (it == attributes.end()) {
		std::cout << "[Error] attributes " << name << " doesn't exist in program" << std::endl;
		return -1;
	}
	return it->second;
}

void Shader::setAttribute(const std::string& name,
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

void Shader::setAttribute(const std::string& name,
                                 GLint size,
                                 GLsizei stride,
                                 GLuint offset,
                                 GLboolean normalize) {
  setAttribute(name, size, stride, offset, normalize, GL_FLOAT);
}

void Shader::setAttribute(const std::string& name,
                                 GLint size,
                                 GLsizei stride,
                                 GLuint offset,
                                 GLenum type) {
  setAttribute(name, size, stride, offset, false, type);
}

void Shader::setAttribute(const std::string& name,
                                 GLint size,
                                 GLsizei stride,
                                 GLuint offset) {
  setAttribute(name, size, stride, offset, false, GL_FLOAT);
}

void Shader::setUniform(const std::string& name,
                               float x,
                               float y,
                               float z) {
  glUniform3f(getUniform(name), x, y, z);
}

void Shader::setUniform(const std::string& name, const vec3& v) {
  glUniform3fv(getUniform(name), 1, value_ptr(v));
}

void Shader::setUniform(const std::string& name, const dvec3& v) {
  glUniform3dv(getUniform(name), 1, value_ptr(v));
}

void Shader::setUniform(const std::string& name, const vec4& v) {
  glUniform4fv(getUniform(name), 1, value_ptr(v));
}

void Shader::setUniform(const std::string& name, const dvec4& v) {
  glUniform4dv(getUniform(name), 1, value_ptr(v));
}

void Shader::setUniform(const std::string& name, const dmat4& m) {
  glUniformMatrix4dv(getUniform(name), 1, GL_FALSE, value_ptr(m));
}

void Shader::setUniform(const std::string& name, const mat4& m) {
  glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, value_ptr(m));
}

void Shader::setUniform(const std::string& name, const mat3& m) {
  glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, value_ptr(m));
}

void Shader::setUniform(const std::string& name, float val) {
  glUniform1f(getUniform(name), val);
}

void Shader::setUniform(const std::string& name, int val) {
  glUniform1i(getUniform(name), val);
}


void Shader::bind() const {
  glUseProgram(program);
}
void Shader::unbind() const {
  glUseProgram(0);
}

