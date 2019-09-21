#include "Light.hpp"



namespace yare { namespace graphics {


	void PointLight::setPosition(glm::vec3& position)
	{
		_position = position;
	}
	void PointLight::setAmbient(glm::vec3& ambient)
	{
		_ambient = ambient;
}
	void PointLight::setDiffuse(glm::vec3& diffuse)
	{
		_diffuse = diffuse;
	}
	void PointLight::setSpecular(glm::vec3& specular)
	{
		_specular = specular;
	}
	void PointLight::setAttenuation(glm::vec3& attenuation)
	{
		_attenuation = attenuation;
	}

	void PointLight::setCutOff(float inner, float outer)
	{
		_innerCutOff = inner;
		_outerCutOff = outer;
	}

	void PointLight::loadUniforms(UniformBlock& uniforms, int lightIndex) const
	{
		const std::string elementStr = "point_lights[" + std::to_string(lightIndex) +"]";
		uniforms.setUniform(elementStr + ".position", _position);
		uniforms.setUniform(elementStr + ".ambient", _ambient);
		uniforms.setUniform(elementStr + ".diffuse", _diffuse);
		uniforms.setUniform(elementStr + ".specular", _specular);
		uniforms.setUniform(elementStr + ".attenuation", _attenuation);

	}
	////////////////// directional /////////////////////////////
	
	void DirectionalLight::loadUniforms(UniformBlock& uniforms, int lightIndex) const
	{
		const std::string elementStr = "directional_lights[" + std::to_string(lightIndex) + "]";
		uniforms.setUniform(elementStr + ".direction", _direction);
		uniforms.setUniform(elementStr + ".ambient", _ambient);
		uniforms.setUniform(elementStr + ".diffuse", _diffuse);
		uniforms.setUniform(elementStr + ".specular", _specular);
	}
	void DirectionalLight::setDirection(glm::vec3& position) 
	{}
	void DirectionalLight::setAmbient(glm::vec3& ambient) 
	{}
	void DirectionalLight::setDiffuse(glm::vec3& diffuse) 
	{}
	void DirectionalLight::setSpecular(glm::vec3& specular)
	{}
	



} }