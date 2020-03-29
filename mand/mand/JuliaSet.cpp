#include "JuliaSet.h"

JuliaSet::JuliaSet(const short& unit)
	: FractalSet(unit, "julia"s) 
{
	shift_x_ = 200;
}

void JuliaSet::Reset() {
	FractalSet::Reset();
	shift_x_ = 200;
}

int JuliaSet::fun_(long double x, long double y, const long double const_x, const long double const_y) const
{
	std::complex<long double> z(x, y);
	std::complex<long double> c(const_x, const_y);
	short i(0);
	while (i++ < max_iterations_) {
		if (z.real() * z.real() + z.imag() * z.imag() > 4)
			return i;
		z = z * z + c;
	}
	return 0;
}