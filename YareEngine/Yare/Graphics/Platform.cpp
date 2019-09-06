#include "Platform.hpp"


YARE_GRAPHICS_MODULE_BEG


Platform& Platform::Current()
{
	static Platform _currentPlatform;
	return _currentPlatform;
}

Platform::Platform()
{
	_renderer = PlatformRenderer::None;
}

YARE_GRAPHICS_MODULE_END