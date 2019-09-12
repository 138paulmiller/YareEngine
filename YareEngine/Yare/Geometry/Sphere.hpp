#pragma once 
#include "Geometry.hpp"
#include "../Graphics/Renderer.hpp"

YARE_GEOMETRY_MODULE_BEG
	
class Sphere
{
public:
	static std::unique_ptr<graphics::VertexArray> CreateVertexArray(float radius, int xSectors, int ySectors);
	//bool intersects(const Line & line);
	//bool intersects(const Sphere & sphere);

	glm::vec3 _position;
	float  _radius;
	//
};

YARE_GEOMETRY_MODULE_END