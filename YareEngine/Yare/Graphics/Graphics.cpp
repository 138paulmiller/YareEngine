
#include "Graphics.hpp"
#include "../Renderer.hpp"
//Graphics Module - Singleton
namespace yare
{
	Graphics gGraphics;
	void Graphics::Setup(RenderAPI api)
	{
		gGraphics._renderAPI = api;
	}
	RenderAPI Graphics::GetRenderAPI() 
	{ 
		return gGraphics._renderAPI;
	}

}
