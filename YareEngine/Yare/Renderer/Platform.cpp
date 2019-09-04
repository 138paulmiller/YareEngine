#include "Platform.hpp"

static Platform _currentPlatform = Platform::None;

void SetPlatform(Platform platform)
{
	_currentPlatform = platform;
}

Platform CurrentPlatform()
{
	return _currentPlatform;
}