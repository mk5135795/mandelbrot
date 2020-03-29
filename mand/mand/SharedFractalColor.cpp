#include "SharedFractalColor.h"

SharedFractalColor::SharedFractalColor(std::shared_ptr<FractalColor> fc_obj)
	: fc_{ fc_obj }
{}

void SharedFractalColor::Set(std::shared_ptr<FractalColor> fc_obj) {
	std::lock_guard<std::mutex> lk(mutex_);
	fc_ = std::move(fc_obj);
}

std::shared_ptr<FractalColor> SharedFractalColor::Get() {
	return std::shared_ptr<FractalColor>(fc_);
}

std::unique_lock<std::mutex> SharedFractalColor::Lock() {
	return std::unique_lock<std::mutex>(mutex_);
}

inline uint32_t SharedFractalColor::Convert(int val) const {
	return fc_->Convert(val);
}

std::string SharedFractalColor::PrintInfo() const {
	return fc_->PrintInfo();
}