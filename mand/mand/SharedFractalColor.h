#pragma once

#include "FractalColor.h"
#include <mutex>

class SharedFractalColor
	: FractalColor
{
public:
	SharedFractalColor(std::shared_ptr<FractalColor> fc_obj = nullptr);

	void Set(std::shared_ptr<FractalColor> fc_obj);
	std::shared_ptr<FractalColor> Get();

	std::unique_lock<std::mutex> Lock();

	inline uint32_t Convert(int val) const;
	std::string PrintInfo() const;

private:
	std::shared_ptr<FractalColor> fc_;
	std::mutex mutex_;
};

