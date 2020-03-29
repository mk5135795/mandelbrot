#pragma once

#include <vector>
#include <complex>
#include <string>

using namespace std::string_literals;

class FractalSet
{
public:
	FractalSet(const short& unit, const std::string&& name);

	void Generate(std::vector<int>& v_iters, const FractalSet& kObj, short y) const;

	void SetShift(double x, double y);
	void SetScale(double scale);
	void SetMaxIter(short max_iter);

	void ChangeShift(double mult_x, double add_x, double mult_y, double add_y);
	void ChangeScale(double mult, double add);
	void ChangeMaxIter(int mult, int add);

	short GetWidth() const;
	short GetHeight() const;
	double GetShiftX() const;
	double GetShiftY() const;
	double GetScale() const;
	short GetMaxIter() const;
	std::string GetName() const;
	long double GetPosX() const;
	long double GetPosY() const;

	virtual void Reset();
	std::string Info() const;
protected:
	virtual inline int fun_(long double x, long double y, const long double const_x, const long double const_y) const = 0;

	const std::string name_;

	const short width_;
	const short height_;

	double shift_x_;
	double shift_y_;
	double scale_;
	short max_iterations_;
};

