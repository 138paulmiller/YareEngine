#pragma once
#include <Yare/Graphics/VertexArray.hpp>

YARE_GRAPHICS_MODULE_BEG

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray();
	virtual ~OpenGLVertexArray();

	virtual void bind() const override;
	virtual void unbind() const override;
	virtual void addVertexBuffer(VertexBuffer * buffer) override;
protected : 
private:
	unsigned int _vao; //handle to the vertex array object
	//For each element describes and attribute. 
	//This holds the next available attribute location
	unsigned int _vertexAttributeIndex; 
	
};


YARE_GRAPHICS_MODULE_END