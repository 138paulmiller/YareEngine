
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Renderer.hpp"
/*

	Primitives are abstracted vertex arrays. There are not meshes as they do not store shader 
*/

//Vertex must have position ,normal and uv attributes
YARE_GRAPHICS_MODULE_BEG


struct SphereVertex {
	glm::vec3 position, normal;
	glm::vec2 uv;

};

class SphereMesh : public Renderable
{
public:

	SphereMesh(
		float radius = 10,
		int sectors = 20);
	~SphereMesh();
	void render(const Renderer * renderer) override ;
	glm::mat4& getModel();
	void setModel(glm::mat4& model);
	
	void setShader(const std::shared_ptr<Shader> & shader);
	void setTexture(const std::shared_ptr<Texture> & texture, int i);

	//rebuild sphere option with different radius and sector?
private:
	std::unique_ptr<VertexArray>  _vertexArray;
	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Texture> _textures[TEXTURE_MAX];

	glm::mat4 _model;
	RenderCommand _command;

};


YARE_GRAPHICS_MODULE_END