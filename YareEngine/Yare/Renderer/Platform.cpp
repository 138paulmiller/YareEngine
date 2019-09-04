#include "Platform.hpp"
namespace yare
{


	Platform& Platform::Current()
	{
		static Platform _currentPlatform;
		return _currentPlatform;
	}

	Platform::Platform()
	{
		_renderer = PlatformRenderer::None;
	}

}