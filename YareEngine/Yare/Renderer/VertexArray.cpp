
#include "Platform.hpp"
#include "Error.hpp"
#include "OpenGL/OpenGLVertexArray.hpp"
namespace yare
{

VertexArray* VertexArray::Create()
{
	switch (Platform::Current().getRenderer())
	{
	case PlatformRenderer::OpenGL: return new OpenGLVertexArray();
	default: YARE_ASSERT(false, "No Render platform selected! "); return 0;
	}
}

VertexArray::~VertexArray() 
{

	_vertexBuffers.clear();
}

void VertexArray::addVertexBuffer(VertexBuffer * buffer)
{
//If the buffer being added is an index buffer. reset the indexbufferreference
	_vertexBuffers.push_back(std::unique_ptr<VertexBuffer>(buffer) );
}
const std::vector<std::unique_ptr<VertexBuffer>>& VertexArray::getVertexBuffers() const
{
	return _vertexBuffers;
}

void VertexArray::setIndexBuffer( IndexBuffer * buffer)
{
	_indexBuffer.reset(buffer);
	
}


const std::unique_ptr<IndexBuffer>& VertexArray::getIndexBuffer() const
{
	return _indexBuffer;
}

}