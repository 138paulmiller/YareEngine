#pragma once
#include  <Yare/Renderer/Buffer.hpp>

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
	//opengl buffer handle
	unsigned int _buffer;
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
	//opengl buffer handle
	unsigned int _buffer;
	unsigned int _count;
};

