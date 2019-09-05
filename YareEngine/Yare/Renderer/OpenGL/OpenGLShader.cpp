
#include "OpenGLShader.hpp"

#include "OpenGLError.hpp"
#include "OpenGLCommon.hpp"


#include <cstdlib>
#include <fstream>

#include <iostream>
#include <stdexcept>
#include <vector>
namespace yare
{


OpenGLShader::OpenGLShader()
	:stages{0}  
{
  program = 0;
  isValid = false;
}

OpenGLShader::~OpenGLShader()
{
	destroy();
}

void OpenGLShader::destroy()
{
	if (program == 0) return;
	glUseProgram(0);
	isValid = false;
	uniforms.clear();
	//Link the compiled stages
	for (int i = 0; i < SHADER_STAGE_COUNT; i++)
	{
		glDetachShader(program, stages[i]);
	}
	glDeleteProgram(program);
}
unsigned int  OpenGLShader::compileStage(const std::string& source, GLenum type)
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
	OpenGLCheckError();
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



void OpenGLShader::compile(const std::string& vertSource, const std::string& fragSource)
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
	else
	{
		std::cout << "Compiled!";	
	}
	OpenGLCheckError();


	cacheAttributeAndUniforms();
}


void OpenGLShader::cacheAttributeAndUniforms()
{
	GLint i;
	GLint count;
	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)
	// maximum name length is 16
	GLchar name[16]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(program, (GLuint)i, sizeof(name) / sizeof(GLchar), &length, &size, &type, name);
		//cache its location
		uniforms[name] = glGetUniformLocation(program, name);
		printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
	}
}

void OpenGLShader::bind() const {
	glUseProgram(program);
}
void OpenGLShader::unbind() const {
	glUseProgram(0);
}

int OpenGLShader::getUniform(const std::string& name)
{
  auto it = uniforms.find(name);
  if (it == uniforms.end()) {
 	std::cout << "[Error] uniform " << name << " doesn't exist in program" << std::endl;
    return -1;

  } 
  return it->second;
}


void OpenGLShader::setUniform(const std::string& name, const glm::vec3& v) {
  glUniform3fv(getUniform(name), 1, glm::value_ptr(v));
}

void OpenGLShader::setUniform(const std::string& name, const glm::dvec3& v) {
  glUniform3dv(getUniform(name), 1, glm::value_ptr(v));
}

void OpenGLShader::setUniform(const std::string& name, const glm::vec4& v) {
  glUniform4fv(getUniform(name), 1, glm::value_ptr(v));
}

void OpenGLShader::setUniform(const std::string& name, const glm::dvec4& v) {
  glUniform4dv(getUniform(name), 1, glm::value_ptr(v));
}

void OpenGLShader::setUniform(const std::string& name, const glm::dmat4& m) {
  glUniformMatrix4dv(getUniform(name), 1, GL_FALSE, glm::value_ptr(m));
}

void OpenGLShader::setUniform(const std::string& name, const glm::mat4& m) {
  glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(m));
}

void OpenGLShader::setUniform(const std::string& name, const glm::mat3& m) {
  glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(m));
}

void OpenGLShader::setUniform(const std::string& name, float val) {
  glUniform1f(getUniform(name), val);
}

void OpenGLShader::setUniform(const std::string& name, int val) {
  glUniform1i(getUniform(name), val);
}


}