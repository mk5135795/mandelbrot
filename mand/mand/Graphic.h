#pragma once

#include <chrono>
#include <SFML/Graphics.hpp>
#include "Fractals.h"
#include "SharedFractalColor.h"

class Graphic
{
public:
	Graphic(const short unit, std::shared_ptr<SharedFractalColor> color_obj);

	void SetUpdate();
	void SetChanged();
	void ToggleAutoupdate();
	void ToggleGrid();

	sf::Texture& GetTexture();

	void SetJulia(bool show, bool control, bool reference);
	void SetMandelbrot(bool show, bool control, bool reference);

	void ChangePosition(float x, float y);
	void ChangeIterations(int n);
	void ChangeZoom(float mult);

	void Reset(bool all);
	void Update();
	std::string ShowInfo(std::shared_ptr<SharedFractalColor> fc_obj);

	void Draw(sf::RenderWindow& window);

private:
	void GenGrid_();
	void ScaleBox_();

	const short width_;
	const short height_;

	float move_[2];
	float scale_;

	std::chrono::duration<double> time_;

	bool change_;
	bool update_;
	bool autoupdate_;
	bool info_;
	bool grid_;

	FractalSetGraphic fr_graphic_;

	FractalSet* show_;
	FractalSet* control_;
	FractalSet* reference_;

	MandelbrotSet mandelbrot_;
	JuliaSet julia_;
	Point zero_;

	sf::RectangleShape rec_box_;
	sf::Texture tex_mand_;
	sf::Texture tex_grid_;
	sf::Sprite spr_grid_;
};

