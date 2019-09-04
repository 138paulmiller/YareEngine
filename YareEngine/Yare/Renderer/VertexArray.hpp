#pragma once
#include <memory>
#include "Buffer.hpp"
class VertexArray 
{
public:

	static VertexArray* Create();

	virtual ~VertexArray();

	virtual void bind() const  =0;
	virtual void unbind() const = 0;

	virtual void addVertexBuffer(VertexBuffer * buffer);
	virtual const std::vector<std::unique_ptr<VertexBuffer>>& getVertexBuffers() const;

	virtual void setIndexBuffer(IndexBuffer * buffer);
	const std::unique_ptr<IndexBuffer>& getIndexBuffer() const;

private:
	std::vector< std::unique_ptr<VertexBuffer>> _vertexBuffers;
	std::unique_ptr<IndexBuffer> _indexBuffer;
};
