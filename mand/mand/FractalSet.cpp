#include "FractalSet.h"

#include <functional>
#include <algorithm>

FractalSet::FractalSet(const short& unit, const std::string&& name) 
	: name_{ name }, width_{ 3 * unit }, height_{ 2 * unit }, 
	shift_x_{ 0 }, shift_y_{ 0 }, scale_{ 1 }, max_iterations_{ 100 }
{}

void FractalSet::Generate(std::vector<int>& v_iters, const FractalSet& kObj, short y) const
{
	const long double const_x = kObj.GetPosX();
	const long double const_y = kObj.GetPosY();
	const long double mult_x = 3. / (width_ * scale_);
	const long double add_x = mult_x * (shift_x_ - 2. / 3 * width_);

	long double tx = 0;
	const long double ty = 2 * (y + shift_y_ - height_ / 2) / (height_ * scale_);

	//*
	std::for_each(
		v_iters.begin(),
		v_iters.begin() + width_,
		[&](int& val)
	{
		val = fun_(tx + add_x, ty, const_x, const_y);
		tx += mult_x;
	});
	//*/
	/*
	for (int& val : v_iters)
	{
		val = fun_(tx + add_x, ty, const_x, const_y);
		tx += mult_x;
	}
	//*/
	/*
	for (auto it = v_iters.begin(); it != v_iters.end(); it++)
	{
		*it = fun_(tx + add_x, ty, const_x, const_y);
		tx += mult_x;
	}
	//*/
}

void FractalSet::SetShift(double x, double y) {
	shift_x_ = x;
	shift_y_ = y;
}

void FractalSet::SetScale(double scale) { 
	scale_ = scale; 
}

void FractalSet::SetMaxIter(short max_iter) {
	max_iterations_ = max_iter;
}

void FractalSet::ChangeShift(double mult_x, double add_x, double mult_y, double add_y) {
	shift_x_ = mult_x * shift_x_ + add_x;
	shift_y_ = mult_y * shift_y_ + add_y;
}

void FractalSet::ChangeScale(double mult, double add) {
	scale_ = mult * scale_ + add;
	if (scale_ < 0)
		scale_ = 1;
}

void FractalSet::ChangeMaxIter(int mult, int add) {
	max_iterations_ = mult * max_iterations_ + add;
	if (max_iterations_ < 1)
		max_iterations_ = 1;
}

short FractalSet::GetWidth() const {
	return width_; 
}

short FractalSet::GetHeight() const {
	return height_; 
}

double FractalSet::GetShiftX() const {
	return shift_x_; 
}

double FractalSet::GetShiftY() const {
	return shift_y_; 
}

double FractalSet::GetScale() const {
	return scale_;
}

short FractalSet::GetMaxIter() const {
	return max_iterations_;
}

std::string FractalSet::GetName() const {
	return name_;
}

long double FractalSet::GetPosX() const {
	/*
	 * w/2 + x	=> pos from center
	 * /w		=> norm x = [0; 1]
	 * *3 -2	=> x = [-2; 1]
	 * /sc		=> zoom
	 */
	return static_cast<long double>((3. * (width_ / 2 + shift_x_) / width_ - 2) / scale_);
}

long double FractalSet::GetPosY() const {
	return static_cast<long double>((2. * (height_ / 2 + shift_y_) / height_ - 1) / scale_);
}

void FractalSet::Reset() {
	shift_x_ = 0;
	shift_y_ = 0;
	scale_ = 1;
	max_iterations_ = 100;
}

std::string FractalSet::Info() const {
	std::string str(name_ + '\n');
	str += "position: [ " + std::to_string(GetPosX()) + "; " + std::to_string(GetPosY()) + " ]\n";
	str += "shift:    [ " + std::to_string(shift_x_) + "; " + std::to_string(shift_y_) + " ]\n" ;
	str += "scale:    " + std::to_string(scale_) + '\n';
	str += "max iter: " + std::to_string(max_iterations_) + '\n';
	str += '\n';
	return str;
}
