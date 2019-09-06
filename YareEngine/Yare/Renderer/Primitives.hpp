
#pragma once
#include <vector>
#include <glm/glm.hpp>

//Vertex must have position ,normal and uv attributes
struct SphereVertex {
	glm::vec3 position, normal;
	glm::vec2 uv;

};
void CreateSphere(
	std::vector<SphereVertex> & vertices,
	std::vector<unsigned int> & indices,
	float radius,
	int sectors
);