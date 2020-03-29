#include "BurningShipSet.h"

BurningShipSet::BurningShipSet(const short& unit)
	: FractalSet(unit, "julia"s)
{
	scale_ = .5;
}

void BurningShipSet::Reset() {
	FractalSet::Reset();
	scale_ = .5;
}

int BurningShipSet::fun_(long double x, long double y, const long double const_x, const long double const_y) const
{
	std::complex<long double> z(const_x, const_y);
	std::complex<long double> c(x, y);
	short i(0);
	while (i++ < max_iterations_) {
		if (z.real() * z.real() + z.imag() * z.imag() > 4)
			return i;
		z.real(abs(std::real(z)));
		z.imag(abs(std::imag(z)));
		z = z * z + c;
	}
	return 0;
}