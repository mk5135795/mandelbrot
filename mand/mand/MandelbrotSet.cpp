#include "MandelbrotSet.h"

MandelbrotSet::MandelbrotSet(const short& unit)
	: FractalSet(unit, "mandelbrot"s)
{
	shift_x_ = -100;
}

void MandelbrotSet::Reset() {
	FractalSet::Reset();
	shift_x_ = -100;
}

inline int MandelbrotSet::fun_(long double x, long double y, const long double const_x, const long double const_y) const
{
	/* no std::complex
	long double re = 0;
	long double im = 0;
	long double tmp;
	int i(0);
	while (i++ < max_iterations_) {
		if (re * re + im * im > 4)
			return i;
		tmp = re * re - im * im + x;
		im = 2 * re * im + y;
		re = tmp;
	}
	return 0;
	//*/
	
	std::complex<long double> z(const_x, const_y);
	std::complex<long double> c(x, y);
	//long double re;
	//long double im;
	int i(0);
	while (i++ < max_iterations_) {
		/* org
		if (z.real() * z.real() + z.imag() * z.imag() > 4)
			return i;
		z = z*z + c;
		//*/
		//* less calls
		const long double re = z.real();
		const long double im = z.imag();
		if (re * re + im * im > 4)
			return i;
		z.real(re * re - im * im);
		z.imag(2 * re * im);
		z = z + c;
		//*/
		/* less calc
		re = z.real();
		im = z.imag();
		z.imag(2 * re * im + y);
		re = re * re;
		im = im * im;
		if (re + im > 4)
			return i;
		z.real(re - im + x);
		//*/
	}
	return 0;
}