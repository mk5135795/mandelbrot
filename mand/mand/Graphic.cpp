#include "Graphic.h"

Graphic::Graphic(const short unit, std::shared_ptr<SharedFractalColor> color_obj)
	: width_{ 3 * unit }, height_{ 2 * unit }, move_{ 0, 0 }, scale_(1), time_{ 0 },
	change_{ true }, update_{ true }, autoupdate_{ false }, info_{ true }, grid_{ true }, 
	fr_graphic_{ 3 * unit, 2 * unit, color_obj}, 
	mandelbrot_{ unit }, julia_{ unit }, zero_{ unit }
{
	show_ = &mandelbrot_;
	control_ = &mandelbrot_;
	reference_ = &zero_;

	rec_box_.setOutlineThickness(1);
	rec_box_.setOutlineColor(sf::Color::White);
	rec_box_.setFillColor(sf::Color::Transparent);
	ScaleBox_();

	tex_mand_.create(width_, height_);

	GenGrid_();
	spr_grid_.setTexture(tex_grid_);
}

void Graphic::SetUpdate() {
	update_ = true;
}

void Graphic::SetChanged() {
	change_ = true;
	info_ = true;
}

void Graphic::ToggleAutoupdate() {
	autoupdate_ = !autoupdate_;
	info_ = true;
}

void Graphic::ToggleGrid() {
	grid_ = !grid_;
}

sf::Texture& Graphic::GetTexture() {
	return tex_mand_;
}

void Graphic::SetJulia(bool show, bool control, bool reference) {
	if (show)
		show = &julia_;
	else if (control)
		control = &julia_;
	else if (reference)
		reference = &julia_;
	else {
		show = &julia_;
		control = &julia_;
		reference = &mandelbrot_;
	}
	update_ = true;
	change_ = true;
	info_ = true;
}

void Graphic::SetMandelbrot(bool show, bool control, bool reference) {
	if (show)
		show = &mandelbrot_;
	else if (control)
		control = &mandelbrot_;
	else if (reference)
		reference = &mandelbrot_;
	else {
		show = &mandelbrot_;
		control = &mandelbrot_;
		reference = &zero_;
	}
	update_ = true;
	change_ = true;
	info_ = true;
}

void Graphic::ChangePosition(float x, float y) {
	move_[0] += x;
	move_[1] += y;

	spr_grid_.move(sf::Vector2f(x, y));
	rec_box_.move(sf::Vector2f(x, y));

	change_ = true;
	info_ = true;
}

void Graphic::ChangeIterations(int n) {
	control_->ChangeMaxIter(1, n);

	change_ = true;
	info_ = true;
}

void Graphic::ChangeZoom(float mult) {
	scale_ *= mult;

	if (control_ == show_) {
		ScaleBox_();
		change_ = true;
	}
	info_ = true;

	return;
}

void Graphic::Reset(bool all) {
	if (!all) {
		control_->Reset();
		return;
	}

	mandelbrot_.Reset();
	julia_.Reset();

	update_ = true;
	change_ = true;
	info_ = true;
}

void Graphic::Update() {
	if (change_ && (update_ || autoupdate_)) {
		control_->ChangeShift(1, move_[0], 1, move_[1]);
		control_->ChangeScale(scale_, 0);
		if (scale_ != 1)
			control_->ChangeShift(scale_, 200 * (1 - static_cast<double>(scale_)), scale_, 0);

		auto time_beg = std::chrono::system_clock::now();
		fr_graphic_.Generate(show_, *reference_);
		auto time_end = std::chrono::system_clock::now();
		time_ = time_end - time_beg;

		fr_graphic_.ApplyTexture(tex_mand_);

		move_[0] = 0;
		move_[1] = 0;
		scale_ = 1;
		spr_grid_.setPosition(move_[0], move_[1]);
		ScaleBox_();

		change_ = false;
		info_ = true;
	}
	update_ = false;
}

std::string Graphic::ShowInfo(std::shared_ptr<SharedFractalColor> fc_obj) {
	if (info_) {
		std::string str;
		str += "show: " + show_->GetName() + '\n';
		str += "ctrl: " + control_->GetName() + '\n';
		str += "ref:  " + reference_->GetName() + '\n';
		str += '\n';

		str += mandelbrot_.Info();
		str += julia_.Info();

		str += fc_obj->PrintInfo();

		str += "\ntime: ";
		str += std::to_string((std::chrono::duration_cast<std::chrono::duration<double>>(time_)).count());
		str += "s\n";

		if (autoupdate_)
			str += "\n[ autoupdate ]\n";
		else if (change_) {
			str += "\n[ modified ]\n";
			str += "move: [" + std::to_string(move_[0]) + "; " + std::to_string(move_[1]) + "]" + '\n';
			str += "scale: " + std::to_string(scale_) + '\n';
		}
		info_ = false;

		return str;
	}
	return std::string("");
}

void Graphic::Draw(sf::RenderWindow& window) {
	if(grid_) {
		window.draw(rec_box_);
		window.draw(spr_grid_);
	}
}

void Graphic::GenGrid_() {
	sf::Uint8* pixels = new sf::Uint8[static_cast<std::size_t>(4) * width_ * height_];
	for (short y(height_ - 1); y >= 0; y--) {
		for (short x(width_ - 1); x >= 0; x--) {
			if (((y - height_ / 2) % 100 == 0 && (x > width_ / 2 - 10 && x < width_ / 2 + 10))
				|| ((y > height_ / 2 - 10 && y < height_ / 2 + 10) && (x - width_ / 2) % 100 == 0))
			{
				pixels[4 * (x + y * width_) + 0] = 255;
				pixels[4 * (x + y * width_) + 1] = 255;
				pixels[4 * (x + y * width_) + 2] = 255;
				pixels[4 * (x + y * width_) + 3] = 255;
			}
			else
				pixels[4 * (x + y * width_) + 3] = 0;
		}
	}
	tex_grid_.create(width_, height_);
	tex_grid_.update(pixels);
	delete[] pixels;
}

void Graphic::ScaleBox_() {
	rec_box_.setPosition(
		width_ / 2 - width_ / static_cast<float>(2 * scale_) + move_[0] + 1,
		height_ / 2 - height_ / static_cast<float>(2 * scale_) + move_[1] + 1);
	rec_box_.setSize(sf::Vector2f(
		static_cast<float>(width_ / scale_) - 2,
		static_cast<float>(height_ / scale_) - 2));
}