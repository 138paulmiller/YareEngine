
#pragma once
#include <glm/glm.hpp>
#include "Renderer.hpp"


class Material
{
	virtual ~Material() = default;
	virtual void preRender() = 0;

};