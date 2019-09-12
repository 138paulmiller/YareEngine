
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Renderer.hpp"
/*

	Primitives are abstracted vertex arrays. There are not meshes as they do not store shader 
*/

//Vertex must have position ,normal and uv attributes
YARE_GRAPHICS_MODULE_BEG





class Mesh : public Renderable
{
public:

	Mesh();
	~Mesh();
	void render(Renderer* renderer) override ;
	
	//Releases owwnership of the VertexArray
	void loadVertexArray(std::unique_ptr<VertexArray>& vertexArray);


	glm::mat4& getModel();
	void setModel(glm::mat4& model);
	
	//Render Command - contains references to 
	inline RenderCommand & getRenderCommand() { return _command; }
private:
	std::unique_ptr<VertexArray>  _vertexArray;
	glm::mat4 _model;
	RenderCommand _command;

};


YARE_GRAPHICS_MODULE_END