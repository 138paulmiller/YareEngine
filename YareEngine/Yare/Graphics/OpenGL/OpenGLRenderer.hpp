#pragma once
#include <Yare/Renderer.hpp>

namespace yare { namespace graphics {  

//Maintains the state (ie pass config
class OpenGLRenderer : public Renderer
{
public:
	
	void renderMesh(const VertexArray * vertexArray) override;
	void renderIndexedMesh(const VertexArray* vertexArray) override;
	void updateState(const RenderState & state) override ;
	void clear(RenderBufferFlag mask) override;
private:
	unsigned int _primitive;
	//add cull face, depoth test vars etc...
};

} } 
