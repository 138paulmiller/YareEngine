#include "Mesh.hpp"
#include <GL/glew.h>

YARE_GRAPHICS_MODULE_BEG


Mesh::Mesh( )
{

	
	_vertexArray.reset(VertexArray::Create());
	_vertexArray->bind();

	//Command
	_command.vertexArray = _vertexArray.get();
	_command.primitive = RenderPrimitive::Triangles;
	_command.mode = RenderMode::IndexedMesh;
}

Mesh::~Mesh()
{
}


void Mesh::loadVertexArray(std::unique_ptr<VertexArray> & vertexArray)
{
	_vertexArray.reset(vertexArray.release());
	_command.vertexArray = _vertexArray.get();
}

void Mesh::render(Renderer* renderer)
{
	_command.uniformBlock.setUniform("model", _model);
	renderer->submit(&_command);
}

glm::mat4& Mesh::getModel()
{
	return _model;
}
void Mesh::setModel(glm::mat4& model)
{
	_model = model;
}



YARE_GRAPHICS_MODULE_END