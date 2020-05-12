#pragma once

class pMath
{
public:
	static const float convertDegreesToRadians(float degree)
	{
		float radians;

		radians = degree / (180.0f / 3.1415926535f);

		return radians;
	}
};
