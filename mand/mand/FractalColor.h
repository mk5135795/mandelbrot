#pragma once

#include <string>
#include <cstdint>

class FractalColor
{
public:
	virtual inline uint32_t Convert(int val) const = 0;
	virtual std::string PrintInfo() const = 0;
	//vitrual std::string Help() =0;
	//virtual void Change(key, bool shift, bool ctrl, bool alt) = 0;
};