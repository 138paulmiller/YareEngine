#pragma once 
#include "Geometry.hpp"
#include "../Renderer.hpp"

YARE_GEOMETRY_MODULE_BEG
	
class Box
{
public:

	static std::unique_ptr<graphics::VertexArray> CreateVertexArray(const glm::vec3 & extent);
	//bool intersects(const Line & line);
	//bool intersects(const Sphere & sphere);

	glm::vec3 _position;
	glm::vec3 _extent; //half of { width, height, length}
	//
};

YARE_GEOMETRY_MODULE_END