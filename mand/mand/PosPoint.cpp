#include "Point.h"

Point::Point()
	: FractalSet(0, "point")
{
	shift_x_ = 200;
}

void Point::Reset() 
{
	FractalSet::Reset();
	shift_x_ = 200;
}

int Point::fun_(long double x, long double y, const long double const_x, const long double const_y) const
{
	return 0;
}