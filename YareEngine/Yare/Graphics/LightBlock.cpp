#pragma once
#include "LightBlock.hpp"

namespace yare {
	namespace graphics {

		void LightBlock::setPointLight(const std::string& name, PointLight* light)
		{
			if(_pointLights.size() < POINT_LIGHT_COUNT)
				_pointLights[name] = light;
		}

		void LightBlock::setDirectionalLight(const std::string& name, DirectionalLight* light)
		{
			if (_pointLights.size() < DIRECTIONAL_LIGHT_COUNT)

				_directionalLights[name] = light;
		}


		void LightBlock::removePointLight(const std::string& name)
		{
			_pointLights.erase(name);
		}
		
		void LightBlock::removeDirectionalLight(const std::string& name)
		{

			_directionalLights.erase(name);
		}


		/*
		Load all lights into shader
		*/
		void LightBlock::loadUniforms(UniformBlock& uniforms) const
		{
			int index = 0;
			for (const std::pair<std::string, PointLight* >& pair : _pointLights)
			{
				pair.second->loadUniforms(uniforms, index++);
			}
			index = 0;
			for (const std::pair<std::string, DirectionalLight* >& pair : _directionalLights)
			{
				pair.second->loadUniforms(uniforms, index++);
			}
		}

	}
}