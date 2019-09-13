#pragma once
#include <Yare/Graphics/Renderer.hpp>

namespace yare { namespace graphics {  

//Maintains the state (ie pass config
class OpenGLRenderer : public Renderer
{
public:
	
	void renderIndexedMesh(const VertexArray * vertexArray) override;
	void updateState(const RenderState & state) override ;

	//add cull face, depoth test vars etc...
};

} } 
