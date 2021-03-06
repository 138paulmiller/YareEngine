#include "OpenGLBuffer.hpp"
#include "OpenGLCommon.hpp"

namespace yare { namespace graphics {   

OpenGLVertexBuffer::OpenGLVertexBuffer(const BufferLayout & layout)
	:_layout(layout)
{

	glCreateBuffers(1, &_vbo);
	OpenGLCheckError();


}
OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_vbo);
}

void OpenGLVertexBuffer::load(const void* data, unsigned int size, bool isDynamic )
{
	//stride is size of vertex block, so total size / styride is number of vertices
	_count = size / _layout.getStride();
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, size, data, isDynamic ? GL_DYNAMIC_DRAW  : GL_STATIC_DRAW);
	OpenGLCheckError();

}

void OpenGLVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}
void OpenGLVertexBuffer::unbind() const
{

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}	
const BufferLayout& OpenGLVertexBuffer::getLayout() const
{
	return _layout ;
}

void OpenGLVertexBuffer::setLayout(const BufferLayout& layout) 
{
	_layout = layout;
}

unsigned int OpenGLVertexBuffer::getVertexCount() const
{
	return _count;
}
////////////////////////////// Index Buffer /////////////////////////////////////////
OpenGLIndexBuffer::OpenGLIndexBuffer()

{
	_count = 0;
	glCreateBuffers(1, &_ibo);
	OpenGLCheckError();

}
OpenGLIndexBuffer::~OpenGLIndexBuffer()
{

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_ibo);
}

void OpenGLIndexBuffer::load(const unsigned int * data, unsigned int size)
{
	_count = size / sizeof(data[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size , data, GL_STATIC_DRAW);
	OpenGLCheckError();

}

unsigned int OpenGLIndexBuffer::getIndexCount() const 
{
	return _count;
}
void OpenGLIndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
}
void OpenGLIndexBuffer::unbind() const
{

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


} } 