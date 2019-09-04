#pragma once
#include <Yare/Renderer/VertexArray.hpp>
#include <memory>

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
	unsigned int _vertexBufferIndex;  //current vertex buffer index., Used to maintain index offset when multiple buffers or woven buffers are added

};
