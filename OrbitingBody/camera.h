#pragma once
#include "pch.h"

struct Camera
{
	const double zoomMin = -12.f;
	const double zoomMax = 12.f;
	const double zoomPow = 1.1f;

	vec<2> zoomPos{ 0.f, 0.f };
	vec<2> movePos{ 0.f, 0.f };

	double zoomMag = 1.f;
	double zoomInt = 0;

	void zoom(double increment)
	{
		zoomInt += increment;
		if (zoomInt > zoomMax)
			zoomInt = zoomMax;
		else
			if (zoomInt < zoomMin)
				zoomInt = zoomMin;

		zoomMag = pow(zoomPow, zoomInt);
	}

	void setZoom(double magnitude)
	{
		zoomMag = magnitude;
	}
};