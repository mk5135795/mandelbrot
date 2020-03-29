#pragma once

#include "FractalSet.h"

class BurningShipSet
	: public FractalSet
{
public:
	BurningShipSet(const short& unit);
	void Reset();
private:
	int fun_(long double x, long double y, const long double const_x, const long double const_y) const;
};

