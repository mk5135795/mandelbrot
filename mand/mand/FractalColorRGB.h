#pragma once

#include "FractalColor.h"

class FractalColorRGB
	: public FractalColor
{
public:
	FractalColorRGB();

	void Next();
	void Prev();

	inline uint32_t Convert(int val) const;

	std::string PrintInfo() const;

private:
	uint32_t Conv_(const uint8_t a, const uint8_t b, const uint8_t c,
		const uint8_t w_max, const uint8_t w_med, const uint8_t w_min) const;

	char type_;
	int mult_;
};

