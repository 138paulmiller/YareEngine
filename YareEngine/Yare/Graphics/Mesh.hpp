
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Material.hpp"

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
	//Must call *Render base class methods
	void preRender() override;
	void postRender() override ;
	
	//Releases owwnership of the VertexArray
	void loadVertexArray(std::unique_ptr<VertexArray>& vertexArray);


	glm::mat4& getModel();
	void setModel(glm::mat4& model);

	const Material* getMaterial();
	void setMaterial(Material * material);

	template <typename VertexType>
	void loadVertices(const std::vector<VertexType> & vertices, const BufferLayout & vertexLayout)
	{
		loadVerticesImpl((void*)vertices.data(), sizeof(VertexType) * vertices.size(), vertexLayout);
	}
	
	void loadIndices(const std::vector<unsigned int> & indices);

	
private:
	void loadVerticesImpl(const void* vertices, const int size, const BufferLayout & vertexLayout);

	glm::mat4 _model;

	std::unique_ptr<VertexArray>  _vertexArray;

	std::shared_ptr<Material> _material;

	
};


} } 