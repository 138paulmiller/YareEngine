#pragma once
#include  <Yare/Renderer/Buffer.hpp>
namespace yare
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const BufferLayout & layout);
	virtual ~OpenGLVertexBuffer();

	void setData(void* data, unsigned int size, bool isDynamic = false) override;
	
	void bind() const override;
	void unbind() const override;

	const BufferLayout& getLayout() const override ;
	void setLayout(const BufferLayout& layout) override;

private: 
	
	//handle to Vertex Buffer Object
	unsigned int _vbo;
	BufferLayout _layout;
};



class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer();
	virtual ~OpenGLIndexBuffer();

	void setData(unsigned int* data, unsigned int count) override;

	unsigned int getIndexCount() const override;
	void bind() const override;
	void unbind() const override;
private:
	//handle to index Buffer Object
	unsigned int _ibo;
	unsigned int _count;
};

}