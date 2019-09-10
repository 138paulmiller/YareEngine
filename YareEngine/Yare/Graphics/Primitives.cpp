#include "Primitives.hpp"
#include <GL/glew.h>

YARE_GRAPHICS_MODULE_BEG

void CreateSphere(
	std::vector<SphereVertex>& vertices,
	std::vector<unsigned int>& indices,
	float radius,
	int sectors
)
{
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
	float s, t;                                     // texCoord

	float sectorStep = 2 * 3.14159 / sectors;
	float stackStep = 3.14159 / sectors;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= sectors; ++i)
	{
		stackAngle = 3.14159 / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
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
			s = (float)j / sectors;
			t = (float)i / sectors;
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
	for (int i = 0; i < sectors; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k1 + 1);
				indices.push_back(k2);
			}

			if (i != (sectors - 1))
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
SphereMesh::SphereMesh(
	float radius,
	int sectors )
{

	//use sphere
	std::vector<SphereVertex> vertices;
	std::vector<unsigned int> indices;
	CreateSphere(vertices, indices, 20, 20);

	_vertexArray.reset(VertexArray::Create());
	_vertexArray->bind();

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

	_vertexArray->addVertexBuffer(vertexBuffer);
	_vertexArray->setIndexBuffer(indexBuffer);
	_vertexArray->unbind();

	//Command
	_command.mode = RenderMode::IndexedMesh;
	_command.vertexArray = _vertexArray.get();
}

SphereMesh::~SphereMesh()
{
}

void SphereMesh::render(const Renderer * renderer)
{
	_command.uniformBuffer.setUniform("model", _model);

	_vertexArray->bind();
	glDrawElements(GL_TRIANGLES,         // mode
		_vertexArray->getIndexBuffer()->getIndexCount(),  // count
		GL_UNSIGNED_INT,      // type
		NULL                  // element array buffer offset
	);
	_vertexArray->unbind();
}

glm::mat4& SphereMesh::getModel()
{
	return _model;
}
void SphereMesh::setModel(glm::mat4& model)
{
	_model = model;
}

void SphereMesh::setShader(const std::shared_ptr<Shader> & shader)
{
	_shader = shader;
	_command.shader = _shader.get();
}

void SphereMesh::setTexture(const std::shared_ptr<Texture> & texture, int i)
{
	_textures[i] = texture;
	_command.textures[i] = _textures[i].get();
}

YARE_GRAPHICS_MODULE_END