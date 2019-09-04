#include "OpenGLVertexArray.hpp"
#include "OpenGLCommon.hpp"
#include <Yare/Renderer/Error.hpp>

static unsigned int BufferElementTypeToOpenGLType(BufferElementType type)
{
	switch (type)
	{
	case BufferElementType::Float:    return GL_FLOAT;
	case BufferElementType::Float2:   return GL_FLOAT;
	case BufferElementType::Float3:   return GL_FLOAT;
	case BufferElementType::Float4:   return GL_FLOAT;
	case BufferElementType::Mat3:     return GL_FLOAT;
	case BufferElementType::Mat4:     return GL_FLOAT;
	case BufferElementType::Int:      return GL_INT;
	case BufferElementType::Int2:     return GL_INT;
	case BufferElementType::Int3:     return GL_INT;
	case BufferElementType::Int4:     return GL_INT;
	}

	YARE_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}


OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &_vao);
	_vertexBufferIndex = 0;
}

OpenGLVertexArray::~OpenGLVertexArray()
{

	glDeleteVertexArrays(1, &_vao);
}

void OpenGLVertexArray::bind() const
{

	glBindVertexArray(_vao);
}

void OpenGLVertexArray::unbind() const
{
	glBindVertexArray(0);

}


void OpenGLVertexArray::addVertexBuffer(VertexBuffer* buffer) 
{
	VertexArray::addVertexBuffer(buffer);

	bind();
	buffer->bind();

	const BufferLayout& layout = buffer->getLayout();

	for (const BufferElement& element : layout.getElements())
	{
		std::cout << element.offset << "\n";
		glEnableVertexAttribArray(_vertexBufferIndex);
		glVertexAttribPointer(_vertexBufferIndex,
			element.componentCount,
			BufferElementTypeToOpenGLType(element.type),
			element.isNormalized ? GL_TRUE : GL_FALSE,
			layout.getStride(),
			reinterpret_cast<void*>(element.offset));
		_vertexBufferIndex++;
	}
	OpenGLCheckError();
}
