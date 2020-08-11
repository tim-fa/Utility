#pragma once

#include <math.h>

#define SP_PI 3.14159265358f

namespace Maths
{

	inline float toRadians(float degrees)
	{
		return (float)(degrees * (SP_PI / 180.0f));
	}

	inline float toDegrees(float radians)
	{
		return (float)(radians * (180.0f / SP_PI));
	}

	inline int sign(float value)
	{
		return (value > 0) - (value < 0);
	}

	inline float sin(float angle)
	{
		return ::sin(angle);
	}

	inline float cos(float angle)
	{
		return ::cos(angle);
	}

	inline float tan(float angle)
	{
		return ::tan(angle);
	}

	inline float sqrt(float value)
	{
		return ::sqrt(value);
	}

	inline float rsqrt(float value)
	{
		return 1.0f / ::sqrt(value);
	}

	inline float asin(float value)
	{
		return ::asin(value);
	}

	inline float acos(float value)
	{
		return ::acos(value);
	}

	inline float atan(float value)
	{
		return ::atan(value);
	}

	inline float atan2(float y, float x)
	{
		return ::atan2(y, x);
	}

	inline float _min(float value, float minimum)
	{
		return (value < minimum) ? minimum : value;
	}

	inline float _max(float value, float maximum)
	{
		return (value > maximum) ? maximum : value;
	}

	inline float clamp(float value, float minimum, float maximum)
	{
		return (value > minimum) ? (value < maximum) ? value : maximum : minimum;
	}
}