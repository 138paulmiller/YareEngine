
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Renderer.hpp"
/*

	Primitives are abstracted vertex arrays. There are not meshes as they do not store shader 
*/

//Vertex must have position ,normal and uv attributes
namespace yare { namespace graphics {  





class Mesh : public Renderable
{
public:

	Mesh();
	~Mesh();
	void render(Renderer* renderer) override ;
	
	//Releases owwnership of the VertexArray
	void loadVertexArray(std::unique_ptr<VertexArray>& vertexArray);

	template <typename VertexType>
	void loadVertices(const std::vector<VertexType> & vertices, const BufferLayout & vertexLayout)
	{
		loadVerticesImpl((void*)vertices.data(), sizeof(VertexType) * vertices.size(), vertexLayout);
	}
	
	void loadIndices(const std::vector<unsigned int> & indices);

	glm::mat4& getModel();
	void setModel(glm::mat4& model);
	
	//Render Command - contains references to 
	inline RenderCommand & getRenderCommand() { return _command; }
private:
	void loadVerticesImpl(const void* vertices, const int size, const BufferLayout & vertexLayout);

	glm::mat4 _model;
	RenderCommand _command;

	// VAO Specs
	std::unique_ptr<VertexArray>  _vertexArray;
	//Who should own data. If neede dot be updated Should be managed by user?
	//std::vector<void> vertices;
	//std::vector<unsigned int>  indices;
};


} } 