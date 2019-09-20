
#pragma once 
#include "../Renderer.hpp"
//Graphics Module - Singleton
namespace yare
{
	class Graphics 
	{
	public:
		//Must call setup before Creating any objects in the graphics dir
		static void Setup(RenderAPI api);

		static RenderAPI GetRenderAPI();

	private : 
		RenderAPI _renderAPI;
	};
}
