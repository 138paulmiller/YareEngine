#include "OpenGLBuffer.hpp"
#include "OpenGLCommon.hpp"

YARE_GRAPHICS_MODULE_BEG 

OpenGLVertexBuffer::OpenGLVertexBuffer(const BufferLayout & layout)
	:_layout(layout)
{
	glCreateBuffers(1, &_vbo);

}
OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_vbo);
}

void OpenGLVertexBuffer::load(void* data, unsigned int size, bool isDynamic )
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, isDynamic ? GL_DYNAMIC_DRAW  : GL_STATIC_DRAW);
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

////////////////////////////// Index Buffer /////////////////////////////////////////
OpenGLIndexBuffer::OpenGLIndexBuffer()

{
	_count = 0;
	glCreateBuffers(1, &_ibo);

}
OpenGLIndexBuffer::~OpenGLIndexBuffer()
{

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_ibo);
}

void OpenGLIndexBuffer::load(unsigned int * data, unsigned int size)
{
	_count = size / sizeof(data[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size , data, GL_STATIC_DRAW);
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


YARE_GRAPHICS_MODULE_END