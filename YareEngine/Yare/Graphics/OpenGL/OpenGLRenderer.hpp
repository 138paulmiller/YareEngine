#pragma once
#include <Yare/Graphics/Renderer.hpp>

YARE_GRAPHICS_MODULE_BEG

//Maintains the state (ie pass config
class OpenGLRenderer : public Renderer
{
public:
	
	void renderIndexedMesh(const VertexArray * vertexArray) override;
	void updateState(const RenderState & state) override ;

	//add cull face, depoth test vars etc...
};

YARE_GRAPHICS_MODULE_END
