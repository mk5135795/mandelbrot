// mand.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//-0.979569 -0.266212

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Complex.h"

int mandelbrot(long double x, long double y, int iterations) {
	Complex z;
	Complex c(x, y);
	int i(0);
	while (i++ < iterations) {
		if (z.Module() > 2)
			return i;
		z = z * z + c;
	}
	return 0;
}

int julia(long double x, long double y, long double cx, long double cy, int iterations) {
	Complex z(x, y);
	Complex c(cx, cy);
	int i(0);
	while (i++ < iterations) {
		if (z.Module() > 2)
			return i;
		z = z * z + c;
	}
	return 0;
}

void set_col(int h, float s, float v, sf::Uint8* pixel) {
	if (h == 0) {
		*(pixel + 0) = 0;
		*(pixel + 1) = 0;
		*(pixel + 2) = 0;
		return;
	}

	if (s > 1) s = 1;
	if (s < 0) s = 0;
	if (v > 1) v = 1;
	if (v < 0) v = 0;
	h = (h + 200) % 360;

	float x = v * s * (1 - abs(fmodf(h / 60., 2) - 1));

	if (h < 60) {
		*(pixel + 0) = 255 * v;
		*(pixel + 1) = 255 * (x + v - v * s);
		*(pixel + 2) = 0;
	}
	else if (h < 120) {
		*(pixel + 0) = 255 * (x + v - v * s);
		*(pixel + 1) = 255 * v;
		*(pixel + 2) = 0;
	}
	else if (h < 180) {
		*(pixel + 0) = 0;
		*(pixel + 1) = 255 * v;
		*(pixel + 2) = 255 * (x + v - v * s);
	}
	else if (h < 240) {
		*(pixel + 0) = 0;
		*(pixel + 1) = 255 * (x + v - v * s);
		*(pixel + 2) = 255 * v;
	}
	else if (h < 300) {
		*(pixel + 0) = 255 * (x + v - v * s);
		*(pixel + 1) = 0;
		*(pixel + 2) = 255 * v;
	}
	else {
		*(pixel + 0) = 255 * v;
		*(pixel + 1) = 0;
		*(pixel + 2) = 255 * (x + v - v * s);
	}
}

void gen_mand(int width, int height, sf::Uint8* pixels, float sat, float val, 
	double sx, double sy, double sc, int iters, float mult)
{
	for (int y(height - 1); y >= 0; y--) {
		for (int x(width - 1); x >= 0; x--) {
			set_col(mult *
				mandelbrot(
					static_cast<long double>((3. * (x + sx) /  width - 2) / sc),
					static_cast<long double>((2. * (y + sy) / height - 1) / sc),
					iters),
				sat, val, &pixels[4 * (x + y * width)]);
			pixels[4 * (x + y * width) + 3] = 255;
		}
	}
}

void gen_julia(int width, int height, sf::Uint8* pixels, float sat, float val,
	double sx, double sy, double sc,
	double mand_x, double mand_y, double mand_sc, int iters, float mult)
{
	for (int y(height - 1); y >= 0; y--) {
		for (int x(width - 1); x >= 0; x--) {
			set_col(mult *
				julia(
					static_cast<long double>((3. * (x + sx) /  width - 2) / sc),
					static_cast<long double>((2. * (y + sy) / height - 1) / sc),
					static_cast<long double>((3. * ( width / 2 + mand_x) /  width - 2) / mand_sc),
					static_cast<long double>((2. * (height / 2 - mand_y) / height - 1) / mand_sc),
					iters),
				sat, val, &pixels[4 * (x + y * width)]);
			pixels[4 * (x + y * width) + 3] = 255;
		}
	}
}

int main()
{
	const int unit   = 400;
	const int width  = 3 * unit;
	const int height = 2 * unit;

	double mand_x = 0;
	double mand_y = 0;
	double mand_sc = 1;
	int mand_it = 100;

	double julia_x = 200;
	double julia_y = 0;
	double julia_sc = 1;
	int julia_it = 100;

	float sat = 0.7;
	float val = 0.9;
	float mult = 1;

	float box_scale = 1;
	int show_type = 0;
	int ctrl_type = 0;

	sf::RenderWindow window(sf::VideoMode(width, height), "mandelbrot");
	sf::RectangleShape rect_box;
	sf::Sprite spr_mand, spr_grid;
	sf::Texture tex_mand, tex_grid;
	sf::Uint8* pixels = new sf::Uint8[width * height * 4];

	window.setFramerateLimit(30);

	rect_box.setOutlineThickness(1);
	rect_box.setOutlineColor(sf::Color::White);
	rect_box.setFillColor(sf::Color::Transparent);

	tex_mand.create(width, height);
	tex_mand.update(pixels);
	spr_mand.setTexture(tex_mand);

	for (int y(height - 1); y >= 0; y--) {
		for (int x(width - 1); x >= 0; x--) {
			if (((y - height / 2)%100 == 0 && (x > width / 2 - 10 && x < width / 2 + 10))
			|| ((y > height / 2 - 10 && y < height / 2 + 10) && (x - width / 2)%100 == 0)) {
				pixels[4 * (x + y * width) + 0] = 255;
				pixels[4 * (x + y * width) + 1] = 255;
				pixels[4 * (x + y * width) + 2] = 255;
				pixels[4 * (x + y * width) + 3] = 255;
			}
			else
				pixels[4 * (x + y * width) + 3] = 0;
		}
	}
	tex_grid.create(width, height);
	tex_grid.update(pixels);
	spr_grid.setTexture(tex_grid);

	bool change = true;
	bool update = true;
	bool autoupdate = false;
	bool info = true;
	bool grid = true;
	int tmp;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonReleased) {
				/*float p1[] = { 0 };
				int iter = 10;
				long double x, y;
				x = static_cast<long double>((3. * (x + mand_x) / width - 2) / mand_sc);
				y = static_cast<long double>((2. * (y + mand_y) / height - 1) / mand_sc);
				Complex z;
				Complex c(x, y);
				int i(0);
				while (i++ < iter) {
					if (z.Module() > 2)
						break;
					z = z * z + c;
					p1[0] = (z.GetRe() * mand_sc + 2) * width / 3. - mand_x;
					p1[1] = (z.GetIm() * mand_sc + 1) * width / 2. - mand_y;
				}*/
			}
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code)
				{
				case sf::Keyboard::W:
					tmp = -100;
					if (event.key.alt)
						tmp = -1;
					else if (event.key.shift) 
						tmp = -10;

					if (ctrl_type == 0) 
						mand_y += tmp;
					else //if (ctrl_type == 1) 
						julia_y += tmp;

					spr_grid.move(sf::Vector2f(0, tmp));
					rect_box.move(sf::Vector2f(0, tmp));

					change = true;
					info = true;
					break;
				case sf::Keyboard::S:
					tmp = 100;
					if (event.key.alt) 
						tmp = 1;
					else if (event.key.shift) 
						tmp = 10; 

					if (ctrl_type == 0) 
						mand_y += tmp;
					else //if (ctrl_type == 1) 
						julia_y += tmp;

					spr_grid.move(sf::Vector2f(0, tmp));
					rect_box.move(sf::Vector2f(0, tmp));

					change = true;
					info = true;
					break;
				case sf::Keyboard::D:
					tmp = 100;
					if (event.key.alt)
						tmp = 1;
					else if (event.key.shift) 
						tmp = 10; 

					if (ctrl_type == 0) 
						mand_x += tmp; 
					else //if (ctrl_type == 1)
						julia_x += tmp; 

					spr_grid.move(sf::Vector2f(tmp, 0));
					rect_box.move(sf::Vector2f(tmp, 0));

					change = true;
					info = true;
					break;
				case sf::Keyboard::A:
					tmp = -100;
					if (event.key.alt) 
						tmp = -1; 
					else if (event.key.shift)
						tmp = -10; 

					if (ctrl_type == 0) 
						mand_x += tmp; 
					else //if (ctrl_type == 1) 
						julia_x += tmp; 

					spr_grid.move(sf::Vector2f(tmp, 0));
					rect_box.move(sf::Vector2f(tmp, 0));

					change = true;
					info = true;
					break;
				case sf::Keyboard::Q:
					if (ctrl_type == 0) {
						mand_sc *= 2;
						mand_x = mand_x * 2 - 200;
						mand_y *= 2;
					}
					else /*if (ctrl_type == 1)*/ {
						julia_sc *= 2;
						julia_x = julia_x * 2 - 200;
						julia_y *= 2;
					}

					box_scale *= 2;
					tmp = ceilf(width / (2. * box_scale)) - floorf(width / (2. * box_scale));
					rect_box.move(sf::Vector2f(
						ceilf( width / (2. * box_scale)),
						ceilf(height / (2. * box_scale))));
					rect_box.setSize(sf::Vector2f(
						ceilf(float( width / box_scale) - tmp + 1), 
						ceilf(float(height / box_scale) - tmp + 1)));

					change = true;
					info = true;
					break;
				case sf::Keyboard::E:
					if (ctrl_type == 0) {
						mand_sc /= 2;
						mand_x = mand_x / 2 + 100;
						mand_y /= 2;
					}
					else /*if (ctrl_type == 1)*/ {
						julia_sc /= 2;
						julia_x = julia_x / 2 + 100;
						julia_y /= 2;
					}

					box_scale /= 2;
					tmp = ceilf(width / (2. * box_scale)) - floorf(width / (2. * box_scale));
					rect_box.move(sf::Vector2f(
						-1 * ceilf( width / (4. * box_scale)),
						-1 * ceilf(height / (4. * box_scale))));
					rect_box.setSize(sf::Vector2f(
						ceilf(float( width / box_scale) - tmp + 1),
						ceilf(float(height / box_scale) - tmp + 1)));

					change = true;
					info = true;
					break;
				case sf::Keyboard::Z:
					if (event.key.alt) {
						if (ctrl_type == 0)
							mand_it += 1;
						else //if (ctrl_type == 1)
							julia_it += 1;
					}
					else if (event.key.shift) {
						if (ctrl_type == 0)
							mand_it += 10;
						else //if (ctrl_type == 1)
							julia_it += 10;
					}
					else {
						if (ctrl_type == 0)
							mand_it += 100;
						else //if (ctrl_type == 1)
							julia_it += 100;
					}

					change = true;
					info = true;
					break;
				case sf::Keyboard::X:
					if (event.key.alt) {
						if (ctrl_type == 0 && mand_it > 1)
							mand_it -= 1;
						else if (ctrl_type == 1 && julia_it > 1)
							julia_it -= 1;
					}
					else if (event.key.shift) {
						if (ctrl_type == 0 && mand_it > 10)
							mand_it -= 10;
						else if (ctrl_type == 1 && julia_it > 10)
							julia_it -= 10;
					}
					else if (mand_it > 100) {
						if (ctrl_type == 0 && mand_it > 100)
							mand_it -= 100;
						else if (ctrl_type == 1 && julia_it > 100)
							julia_it -= 100;
					}

					change = true;
					info = true;
					break;
				case sf::Keyboard::C:
					if (event.key.alt && sat < 1)
						sat += 0.1;
					else if (event.key.shift && val < 1)
						val += 0.1;
					else 
						mult *= 2;

					change = true;
					info = true;
					break;
				case sf::Keyboard::V:
					if (event.key.alt && sat > 0)
						sat -= 0.1;
					else if (event.key.shift && val > 0)
						val -= 0.1;
					else
						mult /= 2;

					change = true;
					info = true;
					break;
				case sf::Keyboard::J:
					if (event.key.shift || !event.key.control) {
						show_type = 1;
						update = true;
						change = true;
					}
					if (!event.key.shift)
						ctrl_type = 1;

					info = true;
					break;
				case sf::Keyboard::M:
					if (event.key.shift || !event.key.control) {
						show_type = 0;
						update = true;
						change = true;
					}
					if (!event.key.shift)
						ctrl_type = 0;

					info = true;
					break;
				case sf::Keyboard::I:
					info = true;
					break;
				case sf::Keyboard::U:
					autoupdate = !autoupdate;
					info = true;
					break;
				case sf::Keyboard::Space:
					update = true;
					break;
				case sf::Keyboard::G:
					grid = !grid;
					break;
				case sf::Keyboard::R:
					if (event.key.control) {
						mand_x = 0;
						mand_y = 0;
						mand_sc = 1;
						mand_it = 100;
						julia_x = 200;
						julia_y = 0;
						julia_sc = 1;
						julia_it = 100;
						sat = 0.7;
						val = 0.9;
						mult = 1;
						box_scale = 1;
						change = true;
						update = true;
					}
					break;
				default:
					break;
				}
			}
		}
		if (change && (update || autoupdate)) {
			if(show_type == 0)
				gen_mand(width, height, pixels, sat, val, mand_x, mand_y, mand_sc, mand_it, mult);
			else //if(show_type == 1)
				gen_julia(width, height, pixels, sat, val, julia_x, julia_y, julia_sc, mand_x, mand_y, mand_sc, julia_it, mult);
			tex_mand.update(pixels);
			spr_grid.setPosition(0, 0);
			rect_box.setPosition(sf::Vector2f(width / 4, height / 4));
			box_scale = 1;
			rect_box.setPosition(sf::Vector2f(0, 0));
			rect_box.setSize(sf::Vector2f(width + 1, height + 1));
			change = false;
			info = true;
		}
		update = false;

		if (info) {
			system("cls");
			std::cout << "show: " << ((show_type == 0) ? "mandelbrot" : "julia") << std::endl;
			std::cout << "ctrl: " << ((ctrl_type == 0) ? "mandelbrot" : "julia") << std::endl;
			std::cout << std::endl;

			std::cout << "mandelbrot" << std::endl;
			std::cout << "position: [ " << (3. * (width / 2 + mand_x) / width - 2) / mand_sc;
			std::cout << ",\t" << (2. * (height / 2 - mand_y) / height - 1) / mand_sc << " ]";
			std::cout << std::endl;
			std::cout << "shift:    [ " << mand_x << "; " << mand_y << " ]" << std::endl;
			std::cout << "scale:    " << mand_sc << std::endl;
			std::cout << "max iter: " << mand_it << std::endl;
			std::cout << std::endl;

			std::cout << "julia" << std::endl;
			std::cout << "position: [ " << (3. * (width / 2 + julia_x) / width - 2) / julia_sc;
			std::cout << ",\t" << (2. * (height / 2 - julia_y) / height - 1) / julia_sc << " ]";
			std::cout << std::endl;
			std::cout << "shift:    [ " << julia_x << "; " << julia_y << " ]" << std::endl;
			std::cout << "scale:    " << julia_sc << std::endl;
			std::cout << "max iter: " << julia_it << std::endl;
			std::cout << std::endl;

			std::cout << "col mult: " << mult << std::endl;
			std::cout << "col sat:  " << sat << std::endl;
			std::cout << "col val:  " << val << std::endl;
			if (autoupdate)
				std::cout << std::endl << "[ autoupdate ]" << std::endl;
			else if (change)
				std::cout << std::endl << "[ modified ]" << std::endl;
			info = false;
		}
		window.clear();
		window.draw(spr_mand);
		if (grid) {
			window.draw(spr_grid);
			window.draw(rect_box);
		}
		window.display();
	}
	delete[] pixels;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
