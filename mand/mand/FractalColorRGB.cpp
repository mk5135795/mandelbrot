#include "FractalColorRGB.h"

FractalColorRGB::FractalColorRGB()
	: type_{ 0 }, mult_{ 10 }
{}

void FractalColorRGB::Next() {
	type_ = ++type_ % 6;
}

void FractalColorRGB::Prev() {
	type_ = (type_ + 5) % 6;
}

inline uint32_t FractalColorRGB::Convert(int val) const {
	val = (val * mult_) % 0x01000000;
	switch (type_)
	{
	case 0: return Conv_((val & 0x0000FF) >> 0, (val & 0x00FF00) >> 8, (val & 0xFF0000) >> 16, 0, 8, 16);
	case 1: return Conv_((val & 0x0000FF) >> 0, (val & 0x00FF00) >> 8, (val & 0xFF0000) >> 16, 0, 16, 8);
	case 2: return Conv_((val & 0x0000FF) >> 0, (val & 0x00FF00) >> 8, (val & 0xFF0000) >> 16, 8, 0, 16);
	case 3: return Conv_((val & 0x0000FF) >> 0, (val & 0x00FF00) >> 8, (val & 0xFF0000) >> 16, 8, 16, 0);
	case 4: return Conv_((val & 0x0000FF) >> 0, (val & 0x00FF00) >> 8, (val & 0xFF0000) >> 16, 16, 0, 8);
	case 5: return Conv_((val & 0x0000FF) >> 0, (val & 0x00FF00) >> 8, (val & 0xFF0000) >> 16, 16, 8, 0);
	}
}

std::string FractalColorRGB::PrintInfo() const {
	std::string str("[rgb]\n");
	switch (type_)
	{
	case 0: str += "r > g >b\n";
	case 1: str += "r > b >g\n";
	case 2: str += "g > r >b\n";
	case 3: str += "g > b >r\n";
	case 4: str += "b > g >r\n";
	case 5: str += "b > r >g\n";
	}
	return str;
}

uint32_t FractalColorRGB::Conv_(const uint8_t a, const uint8_t b, const uint8_t c, 
	const uint8_t w_max, const uint8_t w_med, const uint8_t w_min) const 
{
	if (a > b)
	{
		if (a > c)
		{
			if (b > c)//a > b > c
				return (0xFF << 24) | (a << w_max) | (b << w_med) | (c << w_min);
			else      //a > c > b
				return (0xFF << 24) | (a << w_max) | (c << w_med) | (b << w_min);
		}
		else          //c > a > b
			return (0xFF << 24) | (c << w_max) | (a << w_med) | (b << w_min);
	}
	else
	{
		if (c > a)
		{
			if (b > c)//b > c > a
				return (0xFF << 24) | (b << w_max) | (c << w_med) | (c << w_min);
			else      //c > b > a
				return (0xFF << 24) | (c << w_max) | (b << w_med) | (a << w_min);
		}
		else          //b > a > c
			return (0xFF << 24) | (b << w_max) | (a << w_med) | (c << w_min);
	}
}