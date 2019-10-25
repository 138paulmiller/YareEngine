
#pragma once
#include <map>
#include <vector>
#include <Yare/Graphics/Shader.hpp>
// A shader program is a set of shader (for instance vertex shader + pixel
// shader) defining the rendering pipeline.
//
// This class provide an interface to define the OpenGL uniforms and attributes
// using GLM objects.

namespace yare { namespace graphics {  

class OpenGLShader  : public Shader{
 public:

	// constructor
	OpenGLShader();
	virtual ~OpenGLShader();

	//Destroy program
	void destroy() override;

	void compile(const std::string& vertSource, const std::string& fragSource)override;

	// bind the program
	void bind() const  override;
	void unbind() const override;
	void setUniform(const std::string& name, const glm::vec2& v);
	void setUniform(const std::string& name, const glm::vec3&  v ) override;
	void setUniform(const std::string& name, const glm::dvec3& v ) override;
	void setUniform(const std::string& name, const glm::vec4&  v ) override;
	void setUniform(const std::string& name, const glm::dvec4& v ) override;
	void setUniform(const std::string& name, const glm::dmat4& m ) override;
	void setUniform(const std::string& name, const glm::mat4&  m ) override;
	void setUniform(const std::string& name, const glm::mat3&  m ) override;
	void setUniform(const std::string& name, float val) override;
	void setUniform(const std::string& name, int val)   override;


protected:

	// provide uniform location
	int getUniform(const std::string& name);

	void cacheAttributeAndUniforms();
	unsigned int compileStage(const std::string& source, unsigned int type);

private:

	//name to location Cache to avoid querying drivers for program info
	std::map<std::string, int> _uniforms;
	

	//handles to attached shader stages
	unsigned int  _stages[(int)ShaderStage::Count];
	// opengl id
	unsigned int  _program;

	bool isValid;

};


} } 