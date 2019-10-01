#include "Sphere.hpp"


namespace yare {
	namespace geometry {



using namespace graphics;
struct SphereVertex {
	glm::vec3 position, normal;
	glm::vec2 uv;

};

/*
	ysectors is number of sectors alongs y axis
	

*/
void CreateSphere(
	std::vector<SphereVertex>& vertices,
	std::vector<unsigned int>& indices,
	float radius,
	int ySectors, int xSectors
)
{
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
	float s, t;                                     // texCoord

	float xSectorStep = 2 * 3.14159 / xSectors;
	float yStackStep = 3.14159 / ySectors;
	float xSectorAngle, ySectorAngle;

	for (int i = 0; i <= ySectors; ++i)
	{
		ySectorAngle = 3.14159 / 2 - i * yStackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(ySectorAngle);             // r * cos(u)
		z = radius * sinf(ySectorAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= xSectors; ++j)
		{
			xSectorAngle = j * xSectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(xSectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(xSectorAngle);             // r * cos(u) * sin(v)
			SphereVertex vertex;
			vertex.position.x = x;
			vertex.position.y = y;
			vertex.position.z = z;

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			vertex.normal.x = nx;
			vertex.normal.y = ny;
			vertex.normal.z = nz;

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / ySectors;
			t = (float)i / xSectors;
			vertex.uv.x = s;
			vertex.uv.y = t;

			vertices.push_back(vertex);
		}
	}
	// indices
   //  k1--k1+1
   //  |  / |
   //  | /  |
   //  k2--k2+1
	unsigned int k1, k2;
	for (int i = 0; i < ySectors; ++i)
	{
		k1 = i * (xSectors + 1);     // beginning of current stack
		k2 = k1 + xSectors + 1;      // beginning of next stack

		for (int j = 0; j < xSectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k1 + 1);
				indices.push_back(k2);
			}

			if (i != (ySectors - 1))
			{

				indices.push_back(k1 + 1);
				indices.push_back(k2 + 1);
				indices.push_back(k2);
			}

			//for wireframe mode
			//lineIndices.push_back(k1);
			//lineIndices.push_back(k2);
			//if (i != 0)  
			//{
			//	lineIndices.push_back(k1);
			//	lineIndices.push_back(k1 + 1);
			//}
		}
	}
}

std::unique_ptr<VertexArray> Sphere::CreateVertexArray(float radius, int xSectors, int ySectors)
{

	//use sphere
	std::vector<SphereVertex> vertices;
	std::vector<unsigned int> indices;
	CreateSphere(vertices, indices, radius, xSectors, ySectors);

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