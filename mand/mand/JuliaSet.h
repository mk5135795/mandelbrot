#pragma once

#include "FractalSet.h"

class JuliaSet
	: public FractalSet
{
public:
	JuliaSet(const short& unit);
	void Reset();
private:
	int fun_(long double x, long double y, const long double const_x, const long double const_y) const;
};

