
#include "Graphics.hpp"
#include "Error.hpp"
#include "OpenGL/OpenGLVertexArray.hpp"

namespace yare { namespace graphics { 


VertexArray* VertexArray::Create()
{
	switch (Graphics::GetRenderAPI())
	{
	case RenderAPI::OpenGL: 
		return new OpenGLVertexArray();
	default: YARE_ASSERT(false, "No Render platform selected! "); return 0;
	}
}

VertexArray::~VertexArray() 
{

	_vertexBuffers.clear();
}

void VertexArray::addVertexBuffer(VertexBuffer * buffer)
{
	if (_vertexBuffers.size() == 0) {
		_vertexCount = buffer->getVertexCount();
	}
	else
	{
		_vertexCount += buffer->getVertexCount();
	}
	_vertexBuffers.push_back(std::unique_ptr<VertexBuffer>(buffer) );
	this->attachVertexBuffer(buffer);

}

void VertexArray::setVertexBuffer(VertexBuffer * buffer, int i )
{
	//dec by old amount
	_vertexCount -= _vertexBuffers[i]->getVertexCount();

	//If the buffer being added is an index buffer. reset the indexbufferreference
	_vertexBuffers[i].reset(buffer);
	_vertexCount += buffer->getVertexCount();
	this->attachVertexBuffer(buffer);

}

const std::vector<std::unique_ptr<VertexBuffer>>& VertexArray::getVertexBuffers() const
{
	return _vertexBuffers;
}
const std::unique_ptr<VertexBuffer> & VertexArray::getVertexBuffer(int i) const
{
	return getVertexBuffers()[i];
}

void VertexArray::setIndexBuffer( IndexBuffer * buffer)
{
	_indexBuffer.reset(buffer);
	
}


const std::unique_ptr<IndexBuffer>& VertexArray::getIndexBuffer() const
{
	return _indexBuffer;
}


void VertexArray::setVertexCount(unsigned int vertexCount)
{
	_vertexCount = vertexCount;
}
unsigned int VertexArray::getVertexCount() const
{
	return _vertexCount;
}


} } 