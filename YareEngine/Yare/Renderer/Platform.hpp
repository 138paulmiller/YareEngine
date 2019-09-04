#pragma once

enum class Platform
{
	None = 0,
	OpenGL
};
void SetPlatform(Platform platform);
Platform CurrentPlatform();