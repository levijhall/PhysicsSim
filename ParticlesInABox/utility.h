#pragma once
#include "core.h"

template <typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}