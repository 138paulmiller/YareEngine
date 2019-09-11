#pragma once
#include "Renderer.hpp"
YARE_GRAPHICS_MODULE_BEG


class Config
{
public:
	static Config & Current();
		
	inline void setRenderAPI(RendererAPI rendererAPI) { _rendererAPI = rendererAPI; }
	inline RenderAPI getRenderAPI()	{return _rendererAPI;}
private:

	RenderAPI _rendererAPI;
};

YARE_GRAPHICS_MODULE_END