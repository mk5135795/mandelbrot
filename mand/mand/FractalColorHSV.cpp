#include "FractalColorHSV.h"

#include <iostream>

FractalColorHSV::FractalColorHSV(float multiplier, float saturation, float value)
	: multiplier_{ multiplier }, saturation_{ saturation }, value_{ value }
{
	c_[0] = static_cast<int>(255 * (value_ - value_ * saturation_));
	c_[1] = static_cast<int>(255 * value_ * saturation_);
	c_[2] = static_cast<int>(255 * value_);
}

void FractalColorHSV::SetMultiplier(float multiplier) {
	multiplier_ = multiplier;
}

void FractalColorHSV::SetSaturation(float saturation) {
	if (saturation > 1)
		saturation = 1;
	else if (saturation < 0)
		saturation = 0;
	saturation_ = saturation;
	c_[0] = static_cast<int>(255 * (value_ - value_ * saturation_));
	c_[1] = static_cast<int>(255 * value_ * saturation_);
}

void FractalColorHSV::SetValue(float value) {
	if (value > 1)
		value = 1;
	else if (value < 0)
		value = 0;
	value_ = value;
	c_[0] = static_cast<int>(255 * (value_ - value_ * saturation_));
	c_[1] = static_cast<int>(255 * value_ * saturation_);
	c_[2] = static_cast<int>(255 * value_);
}

void FractalColorHSV::ChangeMultiplier(float mult) {
	multiplier_ *= mult;
}

void FractalColorHSV::ChangeSaturation(float val) {
	saturation_ += val;
	if (saturation_ > 1)
		saturation_ = 1;
	else if (saturation_ < 0)
		saturation_ = 0;
}

void FractalColorHSV::ChangeValue(float val) {
	value_ += val;
	if (value_ > 1)
		value_ = 1;
	else if (value_ < 0)
		value_ = 0;
}

inline uint32_t FractalColorHSV::Convert(int val) const {
	uint32_t col = 255 << 24;
	const int v = static_cast<int>(multiplier_ * val + 200) % 360;
	const uint8_t tmp_x = static_cast<int>(c_[0] + c_[1] * (1 - abs(fmod(v / 60., 2) - 1)));

	switch (v / 60)
	{
	case 0:  col |= (tmp_x <<  8) | (c_[2] <<  0); break;
	case 1:  col |= (tmp_x <<  0) | (c_[2] <<  8); break;
	case 2:  col |= (tmp_x << 16) | (c_[2] <<  8); break;
	case 3:  col |= (tmp_x <<  8) | (c_[2] << 16); break;
	case 4:  col |= (tmp_x <<  0) | (c_[2] << 16); break;
	case 5:  col |= (tmp_x << 16) | (c_[2] <<  0); break;
	}
	return col;
}

std::string FractalColorHSV::PrintInfo() const {
	std::string str("[hsv]\n");
	str += "col mult: " + std::to_string(multiplier_) + '\n';
	str += "col sat:  " + std::to_string(saturation_) + '\n';
	str += "col val:  " + std::to_string(value_) + '\n';
	return str;
}