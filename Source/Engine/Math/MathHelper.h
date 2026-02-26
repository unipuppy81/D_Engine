#pragma once
#include <windows.h>
#include <DirectXMath.h>

namespace D_Engine {
	const float PI = 3.1415926535f;

	inline float ToRadians(float degrees) { return degrees * (PI / 180.0f); }
	inline float ToDegrees(float radians) { return radians * (180.0f / PI); }
}