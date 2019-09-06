#pragma once
#include "Graphics.hpp"
YARE_GRAPHICS_MODULE_BEG

enum class PlatformRenderer
{
	None = 0,
	OpenGL
};


class Platform
{
public:
	static Platform & Current();
		
	Platform();
	inline void setRenderer(PlatformRenderer renderer) { _renderer = renderer; }
	inline PlatformRenderer getRenderer()	{return _renderer;}
private:

	PlatformRenderer _renderer;
};

YARE_GRAPHICS_MODULE_END