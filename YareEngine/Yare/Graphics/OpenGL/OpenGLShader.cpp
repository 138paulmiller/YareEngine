
#include "OpenGLShader.hpp"
#include "OpenGLCommon.hpp"

#include <cstdlib>
#include <fstream>

#include <iostream>
#include <stdexcept>
#include <vector>

namespace yare { namespace graphics {  

OpenGLShader::OpenGLShader()
	:_stages{0}  
{
  _program = 0;
  isValid = false;
}

OpenGLShader::~OpenGLShader()
{
	destroy();
}

void OpenGLShader::destroy()
{
	if (_program == 0) return;
	glUseProgram(0);
	isValid = false;
	_uniforms.clear();
	//Link the compiled stages
	for (int i = 0; i < (int)ShaderStage::Count; i++)
	{
		glDetachShader(_program, _stages[i]);
	}
	glDeleteProgram(_program);
}
unsigned int  OpenGLShader::compileStage(const std::string& source, unsigned int type)
{
	// creation
	GLuint stage = glCreateShader(type);
	OpenGLCheckError();

	if (stage == 0)
		throw std::runtime_error("[Error] Driver could not allocate new shader stage");

	const char* sourceArr = &source[0];
	// code source assignation
	glShaderSource(stage, 1, (const GLchar * *)& sourceArr, NULL);
	OpenGLCheckError();
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
	if (_program != 0) {
		destroy();
	}
	_program = glCreateProgram();
	OpenGLCheckError();

	if (!_program)
		throw std::runtime_error("[Error] Driver could not allocate new shader stage");

	_stages[(int)ShaderStage::Vertex] = compileStage(vertSource, GL_VERTEX_SHADER);
	_stages[(int)ShaderStage::Fragment] = compileStage(fragSource, GL_FRAGMENT_SHADER);
    
	//Link the compiled stages
	for (int i = 0; i < (int)ShaderStage::Count; i++)
	{
		glAttachShader(_program, _stages[i]);
	}

	//link the compile shader stages
	glLinkProgram(_program);
	GLint result;
	glGetProgramiv(_program, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		std::cout << "[Error] linkage error" << std::endl;

		GLsizei logsize = 0;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logsize);

		char* log = new char[logsize];
		glGetProgramInfoLog(_program, logsize, &logsize, log);

		std::cout << log << std::endl;
	}
	else
	{
		std::cout << "Compiled!";	
	}
	OpenGLCheckError();

	glUseProgram(_program);

	cacheAttributeAndUniforms();
	
}


void OpenGLShader::cacheAttributeAndUniforms()
{
	GLint i;
	GLint count;
	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)
	// maximum name length is 16
	int uniformLen = 25;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformLen);
	char *name = new char[uniformLen]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(_program, (GLuint)i, uniformLen / sizeof(GLchar), &length, &size, &type, name);
		//cache its location
		_uniforms[name] = glGetUniformLocation(_program, name);
	}
	delete [] name;
}

void OpenGLShader::bind() const {
	glUseProgram(_program);

}
void OpenGLShader::unbind() const {
	glUseProgram(0);
}

int OpenGLShader::getUniform(const std::string& name)
{
  auto it = _uniforms.find(name);
  if (it == _uniforms.end()) {
 	std::cout << "[Error] uniform " << name << " doesn't exist in program" << std::endl;
    return -1;

  } 
  return it->second;
}

void OpenGLShader::setUniform(const std::string& name, const glm::vec2& v) {
	glUniform2fv(getUniform(name), 1, glm::value_ptr(v));
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



} } 