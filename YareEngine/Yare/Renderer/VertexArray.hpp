#pragma once
#include <memory>
#include "Buffer.hpp"

/*
Example Usage :
	//1. Create VAO first
	VertexArray  * vertexArray = VertexArray::Create();
	vertexArray->bind();
	//2. Then create a VertexBuffer
	BufferLayout vertexLayout = {

		{BufferElementType::Float3, "position"},
		{BufferElementType::Float3, "normal"},
		{BufferElementType::Float4, "color"},
	};
	VertexBuffer * vertexBuffer = VertexBuffer::Create(vertexLayout);
	vertexBuffer->setData(&vertices[0], vertices.size() * sizeof(VertexType));

	//3. Create a index buffer
	IndexBuffer* indexBuffer = IndexBuffer::Create();
	indexBuffer->setData(&indices[0], indices.size() );

	//4. Add the buffers
	vertexArray->addVertexBuffer(vertexBuffer);
	vertexArray->setIndexBuffer(indexBuffer);

	vertexArray->unbind();


*/
namespace yare
{

class VertexArray
{
public:

	static VertexArray* Create();

	virtual ~VertexArray();

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual void addVertexBuffer(VertexBuffer * buffer);
	virtual const std::vector<std::unique_ptr<VertexBuffer>>& getVertexBuffers() const;

	virtual void setIndexBuffer(IndexBuffer * buffer);
	const std::unique_ptr<IndexBuffer>& getIndexBuffer() const;

private:
	std::vector< std::unique_ptr<VertexBuffer>> _vertexBuffers;
	std::unique_ptr<IndexBuffer> _indexBuffer;
};
}