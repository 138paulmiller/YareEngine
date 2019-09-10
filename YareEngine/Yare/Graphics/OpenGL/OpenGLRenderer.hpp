#pragma once
#include <Yare/Graphics/Renderer.hpp>

YARE_GRAPHICS_MODULE_BEG

//Maintains the state (ie pass config
class OpenGLRenderer : public Renderer
{
public:
	
	void renderIndexedMesh(const VertexArray * vertexArray) ;
	void updateState(const RenderState & state) ;

	//add cull face, depoth test vars etc...
};

YARE_GRAPHICS_MODULE_END
