#pragma once

#include "FractalSet.h"

class MandelbrotSet
	: public FractalSet
{
public:
	MandelbrotSet(const short& unit);
	void Reset();
private:
	inline int fun_(long double x, long double y, const long double const_x, const long double const_y)  const;
};