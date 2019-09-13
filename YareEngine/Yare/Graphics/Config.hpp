#pragma once
#include "Renderer.hpp"
namespace yare { namespace graphics {  


class Config
{
public:
	static Config & Current();
		
	inline void setRenderAPI(RendererAPI rendererAPI) { _rendererAPI = rendererAPI; }
	inline RenderAPI getRenderAPI()	{return _rendererAPI;}
private:

	RenderAPI _rendererAPI;
};

} } 