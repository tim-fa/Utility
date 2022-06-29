#include "InternalUtility/Rendering/Color.h"

namespace Rendering
{
unsigned __int8 colorToR(const Color& color)
{
	return (color >> 24) & 0xFF;
}

unsigned __int8 colorToG(const Color& color)
{
	return (color >> 16) & 0xFF;
}

unsigned __int8 colorToB(const Color& color)
{
	return (color >> 8) & 0xFF;
}

unsigned __int8 colorToA(const Color& color)
{
	return (color >> 0) & 0xFF;
}
}