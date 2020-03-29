#pragma once

#include "FractalColor.h"

class FractalColorHSV
	: public FractalColor
{
public:
	FractalColorHSV(float multiplier = 1, float saturation = 1, float value = 1);

	void SetMultiplier(float multiplier);
	void SetSaturation(float saturation);
	void SetValue(float value);

	void ChangeMultiplier(float mult);
	void ChangeSaturation(float val);
	void ChangeValue(float val);

	inline uint32_t Convert(int val) const;

	std::string PrintInfo() const;

private:
	float multiplier_;
	float saturation_;
	float value_;
	uint8_t c_[3];
};