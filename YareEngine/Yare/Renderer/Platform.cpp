#include "Platform.hpp"
namespace yare
{

static Platform _currentPlatform = Platform::None;

void SetPlatform(Platform platform)
{
	_currentPlatform = platform;
}

Platform CurrentPlatform()
{
	return _currentPlatform;
}

}