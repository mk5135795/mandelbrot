#include "FractalSetGraphic.h"

FractalSetGraphic::FractalSetGraphic(short w, short h, std::shared_ptr<SharedFractalColor> color_obj, int workers_num) :
	width_(w), height_(h),
	v_iters_(std::vector< std::vector<int>>(h, std::vector<int>(w, 0))), workers_(workers_num),
	fcolor_{ color_obj }
{
	if(static_cast<std::size_t>(4) * w * h >= SIZE_MAX)
		throw std::overflow_error("max size reached");

	pixels_ = new uint8_t[static_cast<std::size_t>(4) * w * h];
	iters_ = new int[static_cast<std::size_t>(4) * w * h];
}

FractalSetGraphic::~FractalSetGraphic() {
	workers_.End();
	delete[] pixels_;
	delete[] iters_;
}

void FractalSetGraphic::ApplyTexture(sf::Texture& tex) {
	tex.update(pixels_); 
}

void FractalSetGraphic::Generate(FractalSet* fset, FractalSet& obj_arg) {
	for (short j(height_ - 1); j >= 0; j--) {
		//if (j % 2) continue;
		workers_.AddJob([=, &fset, &obj_arg]() {
			fset->Generate(v_iters_[j], obj_arg, j);
			workers_.AddJob([=]() {
				this->SetPixels(j);
			});
		});
	}
	workers_.Start();
	workers_.WaitForEnd();
}

void FractalSetGraphic::SetPixels(short y) const {
	uint8_t* pixel = pixels_ + (4 * (y * static_cast<std::size_t>(width_)));
	for (auto it_row(v_iters_[y].begin()); it_row != v_iters_[y].begin() + width_; it_row++)
	{
		if (*it_row != 0)
			SetPixelColor_(pixel, fcolor_->Convert(*it_row));
		else
			SetPixelColor_(pixel, 0, 0, 0);

		pixel += 4;
	}
}

inline void FractalSetGraphic::SetPixelColor_(uint8_t* p, uint32_t color) {
	*(reinterpret_cast<uint32_t*>(p)) = color;
}

inline void FractalSetGraphic::SetPixelColor_(uint8_t* p, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	const uint8_t arr[4] = { r, g, b, a };
	*(reinterpret_cast<uint32_t*>(p)) = *(reinterpret_cast<const uint32_t*>(arr));
}