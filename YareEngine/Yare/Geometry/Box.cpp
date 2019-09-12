#include "Box.hpp"

YARE_GEOMETRY_MODULE_BEG

using namespace graphics;

struct BoxVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

std::unique_ptr<graphics::VertexArray> Box::CreateVertexArray(const glm::vec3 & extent)
{
	std::vector<BoxVertex> vertices;
	vertices.resize(8);


	/* 
	Extent is half the width
	 5 _________ 6
	  \         |\
	  |\1_______|_\2
	 4\ |_______|7|
 	   \|________\|
	   0          3

		       ^ yextent
		       |
	-xextent <----> +xextent
	*/
	std::cout << extent.x << ", " << extent.z;;
	vertices[0].position = { -extent.x, -extent.y, extent.z };
	vertices[1].position = { -extent.x,  extent.y, extent.z };
	vertices[2].position = {  extent.x,  extent.y, extent.z };
	vertices[3].position = {  extent.x, -extent.y, extent.z };

	vertices[4].position = { -extent.x, -extent.y, -extent.z };
	vertices[5].position = { -extent.x,  extent.y, -extent.z };
	vertices[6].position = { extent.x,  extent.y,  -extent.z };
	vertices[7].position = { extent.x, -extent.y,  -extent.z };

	vertices[0].normal = { -1, -1, -1 };
	vertices[1].normal = { -1,  1, -1 };
	vertices[2].normal = { 1,  1, -1 };
	vertices[3].normal = { 1, -1, -1 };

	vertices[4].normal = { -1, -1,  1 };
	vertices[5].normal = { -1,  1,  1 };
	vertices[6].normal = { 1,  1,  1 };
	vertices[7].normal = { 1, -1,  1 };


	vertices[0].uv = { 0, 0 };
	vertices[1].uv = { 0, 1 };
	vertices[2].uv = { 1, 1 };
	vertices[3].uv = { 1, 0 };

	vertices[4].uv = { 0, 0 };
	vertices[5].uv = { 0, 1 };
	vertices[6].uv = { 1, 1 };
	vertices[7].uv = { 1, 0 };

	std::vector<unsigned int> indices = {
		//4,5,6
		//size +z
		0,1,2,
		2,3,0,
		//side +x
		3,2,6,
		6,7,3,
		//side -z
		7,6,5,
		5,4,7,
		//side -x
		4,5,1,
		1,0,4,
		//side +y
		1,5,6,
		6,2,1,
		//-y
		4,0,3,
		3,7,4
	};

	
	VertexArray * vertexArray = VertexArray::Create();
	vertexArray->bind();

	BufferLayout vertexLayout = {
		{BufferElementType::Float3, "position"},
		{BufferElementType::Float3, "normal"},
		{BufferElementType::Float2, "uv"},
	};
	VertexBuffer* vertexBuffer = VertexBuffer::Create(vertexLayout);
	//vertexBuffer->load(&vertices[0], sizeof(vertices));
	vertexBuffer->load(&vertices[0], vertices.size() * sizeof(vertices[0]));

	IndexBuffer* indexBuffer = IndexBuffer::Create();
	//indexBuffer->load(&indices[0], sizeof(indices));
	indexBuffer->load(&indices[0], indices.size() * sizeof(indices[0]));

	vertexArray->addVertexBuffer(vertexBuffer);
	vertexArray->setIndexBuffer(indexBuffer);
	vertexArray->unbind();
	return std::unique_ptr<VertexArray>(vertexArray);
}

YARE_GEOMETRY_MODULE_END