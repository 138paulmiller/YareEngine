
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "VertexArray.hpp"
#include "Shader.hpp"
/*

	Primitives are abstracted vertex arrays. There are not meshes as they do not store shader 
*/

//Vertex must have position ,normal and uv attributes
YARE_GRAPHICS_MODULE_BEG


struct SphereVertex {
	glm::vec3 position, normal;
	glm::vec2 uv;

};

class SphereMesh
{
public:

	SphereMesh(
		float radius = 10,
		int sectors = 20);
	~SphereMesh();
	void render(const glm::mat4& projection, const glm::mat4& view);
	glm::mat4& getModel();
	void setModel(glm::mat4& model);
	//rebuild sphere option with different radius and sector?
private:
	std::unique_ptr<VertexArray>  _vertexArray;
	glm::mat4 _model;
};


YARE_GRAPHICS_MODULE_END