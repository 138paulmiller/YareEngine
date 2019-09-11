#include "Buffer.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLBuffer.hpp"

YARE_GRAPHICS_MODULE_BEG


////////////////////////// BufferElement  /////////////////////////////////////
BufferElement::BufferElement(BufferElementType type ,
	const std::string& name ,
	bool isNormalized )
	: name(name), type(type), isNormalized(isNormalized), 
	size(0), componentCount(0), offset(0)
{
	switch (type)
	{
	case BufferElementType::Float:   componentCount = 1; size = componentCount * sizeof(float ); break;
	case BufferElementType::Float2:  componentCount = 2; size = componentCount * sizeof(float ); break;
	case BufferElementType::Float3:  componentCount = 3; size = componentCount * sizeof(float ); break;
	case BufferElementType::Float4:  componentCount = 4; size = componentCount * sizeof(float ); break;
	case BufferElementType::Mat3:    componentCount = 9; size = componentCount * sizeof(float ); break;
	case BufferElementType::Mat4:    componentCount = 9; size = componentCount * sizeof(float ); break;
	case BufferElementType::Int:     componentCount = 1; size = componentCount * sizeof(int ); break;
	case BufferElementType::Int2:    componentCount = 2; size = componentCount * sizeof(int); break;
	case BufferElementType::Int3:    componentCount = 3; size = componentCount * sizeof(int ); break;
	case BufferElementType::Int4:    componentCount = 4; size = componentCount * sizeof(int); break;
	default :
		YARE_ASSERT(false, "Invalid Buffer Element Type!");
	}

}



////////////////////////// BufferLayout  /////////////////////////////////////
BufferLayout::BufferLayout(const std::initializer_list<BufferElement> & elements):
	_elements(elements)

{
	//claculate offsets and strides
	unsigned int offset = 0;
	_stride = 0;
	for (auto& element : _elements)
	{
		element.offset = offset;
		offset += element.size;
		_stride += element.size;
	}
}

BufferLayout::BufferLayout(const BufferLayout& other):
	_elements(other._elements),
	_stride(other._stride)

{

}
////////////////////////// Buffer /////////////////////////////////////

VertexBuffer* VertexBuffer::Create(const BufferLayout& layout)
{
	if (layout.getElements().empty())
	{
		YARE_ASSERT(false, " Failed to add Vertex Buffer. You must set the layout!");
		return 0 ;
	}
	switch (Renderer::GetInstance()->getAPI())
	{
	case RenderAPI::OpenGL: 
		return new OpenGLVertexBuffer(layout);
	default: 
		YARE_ASSERT(false, "No Render platform selected! "); return 0;
	}
}


IndexBuffer* IndexBuffer::Create()
{
	switch (Renderer::GetInstance()->getAPI())
	{
	case RenderAPI::OpenGL:
		return new OpenGLIndexBuffer();
	default: 
		YARE_ASSERT(false, "No Render platform selected! "); return 0;
	}
}


YARE_GRAPHICS_MODULE_END