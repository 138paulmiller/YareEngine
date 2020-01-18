#include "Light.hpp"



namespace yare { namespace graphics {


	void PointLight::setPosition(const glm::vec3& position)
	{
		Light::getCamera()->setPosition(position);
		_position = position;
	}
	void PointLight::setAmbient(const glm::vec3& ambient)
	{
		_ambient = ambient;
}
	void PointLight::setDiffuse(const glm::vec3& diffuse)
	{
		_diffuse = diffuse;
	}
	void PointLight::setSpecular(const glm::vec3& specular)
	{
		_specular = specular;
	}
	void PointLight::setAttenuation(const glm::vec3& attenuation)
	{
		_attenuation = attenuation;
	}

	void PointLight::setRadius(float radius)
	{
		_radius =radius;
	}

	void PointLight::unloadUniforms(UniformBlock& uniforms, int lightIndex) const
	{
		const std::string elementStr = "pt_lights[" + std::to_string(lightIndex) +"]";
		if (Light::getCamera())
			uniforms.setUniform(elementStr + ".light_space", Light::getCamera()->getView());


		if (Light::getShadowMap()) {

			uniforms.setUniform(elementStr + ".shadowmap", getShadowMap());
			uniforms.setUniform(elementStr + ".cast_shadow", 1);
		}
		else
		{
			uniforms.setUniform(elementStr + ".cast_shadow", 0);
		}
		uniforms.setUniform(elementStr + ".position", _position);
		uniforms.setUniform(elementStr + ".ambient", _ambient);
		uniforms.setUniform(elementStr + ".diffuse", _diffuse);
		uniforms.setUniform(elementStr + ".specular", _specular);
		uniforms.setUniform(elementStr + ".attenuation", _attenuation);
		uniforms.setUniform(elementStr + ".radius", _radius);
	}
	////////////////// directional /////////////////////////////
	
	void DirectionalLight::unloadUniforms(UniformBlock& uniforms, int lightIndex) const
	{
		const std::string elementStr = "dir_lights[" + std::to_string(lightIndex) + "]";
		if (Light::getCamera())
			uniforms.setUniform(elementStr + ".light_space", Light::getCamera()->getView());

		if (Light::getShadowMap()) {

			uniforms.setUniform(elementStr + ".shadowmap", getShadowMap());
			uniforms.setUniform(elementStr + ".cast_shadow", 1);
		}
		else
		{
			uniforms.setUniform(elementStr + ".cast_shadow", 0);
		}

		uniforms.setUniform(elementStr + ".direction", _direction);
		uniforms.setUniform(elementStr + ".ambient", _ambient);
		uniforms.setUniform(elementStr + ".diffuse", _diffuse);
		uniforms.setUniform(elementStr + ".specular", _specular);
	}
	void DirectionalLight::setDirection(glm::vec3& direction)
	{
		_direction = direction;
		Light::getCamera()->setForward(_direction);
	}

	void DirectionalLight::setPosition(glm::vec3& position)
	{
		_position = position;
		Light::getCamera()->setPosition(position);
	}
	void DirectionalLight::setAmbient(glm::vec3& ambient) 
	{
		_ambient = ambient;
	}
	void DirectionalLight::setDiffuse(glm::vec3& diffuse) 
	{
		_diffuse = diffuse;
	}
	void DirectionalLight::setSpecular(glm::vec3& specular)
	{
		_specular = specular;
	}
	



} }