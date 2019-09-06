

#pragma once
#include "Graphics.hpp"
#include <initializer_list>
#include <string>
#include <vector>

YARE_GRAPHICS_MODULE_BEG

enum class BufferElementType { 
    None, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4 
};
//Used to describe the typoe of element in the buffer
struct BufferElement
{
    std::string name;
    //size of elemtent in bytes, and offset in buffer 
    unsigned int size, offset, componentCount;
    bool isNormalized;
    BufferElementType type;
    BufferElement(  BufferElementType type  = BufferElementType::None, 
                    const std::string &name ="NONE", 
                    bool isNormalized       = false);
    
    
};

//Used top describe to element layout of the buffer
class BufferLayout 
{
public:
	BufferLayout(const std::initializer_list<BufferElement>& elements );
	BufferLayout(const BufferLayout& other);
    
	inline unsigned int getStride() const { return _stride; }
	inline const std::vector<BufferElement> & getElements() const { return _elements; }
private:
    std::vector<BufferElement> _elements;
    unsigned int _stride;

};



//The varying types of buffers
class VertexBuffer 
{
public:
	static VertexBuffer* Create(const BufferLayout& layout);

	virtual ~VertexBuffer() = default;

	//Load raw data into the buffer
	//data is pointer to beginning of data array
	//size is size of buffer in bytes
	virtual void load(void* data, unsigned int size, bool isDynamic = false) = 0;
	virtual void bind() const = 0;
	virtual void unbind()const = 0;

	virtual const BufferLayout& getLayout() const = 0;
	virtual void setLayout(const BufferLayout& layout) = 0;


};

class IndexBuffer 
{
public:
	static IndexBuffer* Create();

	virtual ~IndexBuffer() = default;

	//Load raw data into the buffer
	//data is pointer to beginning of data array
	//size is size of buffer in bytes
	virtual void load(unsigned int* data, unsigned int size) = 0;
	virtual void bind() const = 0;
	virtual void unbind()const = 0;

	virtual unsigned int getIndexCount() const = 0;
};

YARE_GRAPHICS_MODULE_END