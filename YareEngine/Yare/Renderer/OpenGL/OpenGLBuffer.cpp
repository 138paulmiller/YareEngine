#include "OpenGLBuffer.hpp"
#include "OpenGLCommon.hpp"

OpenGLVertexBuffer::OpenGLVertexBuffer(const BufferLayout & layout)
	:_layout(layout)
{
	glCreateBuffers(1, &_buffer);

}
OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &_buffer);
}

void OpenGLVertexBuffer::setData(void* data, unsigned int size, bool isDynamic )
{
	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, isDynamic ? GL_DYNAMIC_DRAW  : GL_STATIC_DRAW);
}

void OpenGLVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
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

////////////////////////////// Index Buffer /////////////////////////////////////////
OpenGLIndexBuffer::OpenGLIndexBuffer()

{
	_count = 0;
	glCreateBuffers(1, &_buffer);

}
OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &_buffer);
}

void OpenGLIndexBuffer::setData(unsigned int * data, unsigned int count)
{
	_count = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

unsigned int OpenGLIndexBuffer::getIndexCount() const 
{
	return _count;
}
void OpenGLIndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
}
void OpenGLIndexBuffer::unbind() const
{

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



