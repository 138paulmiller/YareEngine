#include "Mesh.hpp"
#include "UniformBlock.hpp"
#include <GL/glew.h>

namespace yare { namespace graphics {  


Mesh::Mesh( )
{

	_material = 0;
	_vertexArray.reset(VertexArray::Create());

	Renderable::command.vertexArray = _vertexArray.get();

	//Command
	Renderable::command.vertexArray = _vertexArray.get();
	Renderable::command.state.primitive = RenderPrimitive::Triangles;
	Renderable::command.mode = RenderMode::IndexedMesh;

	_transform = glm::mat4(1);
}

Mesh::~Mesh()
{
}


void Mesh::loadVertexArray(std::unique_ptr<VertexArray> & vertexArray)
{
	_vertexArray.reset(vertexArray.release());
	Renderable::command.vertexArray = _vertexArray.get();
}

void Mesh::loadVerticesImpl(const void* vertices, const int size, const BufferLayout & vertexLayout)
{
	// creation of the vertex array buffer----------------------------------------
//1 . Create VAO, then VBO, then IBO
	_vertexArray->bind();

	if (_vertexArray->getVertexBuffers().size() <= 0 )
	{
		_vertexArray->addVertexBuffer(VertexBuffer::Create(vertexLayout));
	}

	_vertexArray->getVertexBuffer(0)->load(vertices, size);
	_vertexArray->unbind();

}
void Mesh::loadIndices(const std::vector<unsigned int> & indices)
{
	IndexBuffer * indexBuffer = _vertexArray->getIndexBuffer().get();
	_vertexArray->bind();

	if (!indexBuffer)
	{
		_vertexArray->setIndexBuffer(IndexBuffer::Create());
		indexBuffer = _vertexArray->getIndexBuffer().get();
	}
	
	indexBuffer->load(&indices[0], indices.size() * sizeof(unsigned int));
	
	_vertexArray->unbind();
}


void Mesh::onBind() 
{

} 

glm::mat4& Mesh::getTransform()
{
	return _transform;
}
void Mesh::setTransform(glm::mat4& model)
{
	_transform = model;
	Renderable::command.uniforms.setUniform("model", _transform);

}
const Material* Mesh::getMaterial()
{
	return _material;
}
void Mesh::setMaterial(Material* material)
{
	_material = material;
	if (_material)
	{
		//get command details 
		RenderCommand & materialCommand = _material->getCommand();
		
		Renderable::command.lighting = materialCommand.lighting;
		Renderable::command.mode= materialCommand.mode;
		Renderable::command.shader = materialCommand.shader;
		Renderable::command.state = materialCommand.state;

		//loads materials uniforms into uniform block
		_material->unloadUniforms(Renderable::command.uniforms);
	}
}



} } 