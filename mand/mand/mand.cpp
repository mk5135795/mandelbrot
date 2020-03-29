//-0.979569 -0.266212
//-0.228155 -1.11514
//-0.562203 -0.642817
//-0.483161 -0.625537

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Fractals.h"
#include "Graphic.h"

#include "FractalColors.h"

/*
 * tiles in colarr
 * big arr
 * opt move on spr
 *
 * col(num, z)
 */

int main()
{
	const short unit(400);
	const short width(3 * unit);
	const short height(2 * unit);

	std::shared_ptr<FractalColorHSV> hsv_col = std::make_shared<FractalColorHSV>();
	std::shared_ptr<FractalColorRGB> rgb_col = std::make_shared<FractalColorRGB>();
	std::shared_ptr<SharedFractalColor> shr_fc = std::make_shared<SharedFractalColor>(hsv_col);
	shr_fc->Set(rgb_col);
	Graphic g(unit, shr_fc);

	sf::RenderWindow window(sf::VideoMode(width, height), "mandelbrot");
	sf::Sprite spr_mand;

	window.setFramerateLimit(30);

	spr_mand.setTexture(g.GetTexture());

	float mult;
	std::string info;
	while (window.isOpen())	{
		sf::Event event;
		while (window.pollEvent(event))	{
			if (event.type == sf::Event::Closed)
				window.close();

			else if (event.type == sf::Event::KeyPressed) {
				mult = 100;
				if (event.key.shift)
					mult = 10;
				else if (event.key.alt)
					mult = 1;

				switch (event.key.code)
				{
					/*case sf::Keyboard::C:
						if (event.key.alt)
							fr_graphic.ChangeSaturation(float(0.1));
						else if (event.key.shift)
							fr_graphic.ChangeValue(float(0.1));
						else
							fr_graphic.ChangeMult(2);

						g.Changed();
						break;
					case sf::Keyboard::V:
						if (event.key.alt)
							fr_graphic.ChangeSaturation(float(-0.1));
						else if (event.key.shift)
							fr_graphic.ChangeValue(float(-0.1));
						else
							fr_graphic.ChangeMult(0.5);

						g.Changed();
						break;*/

				case sf::Keyboard::W: g.ChangePosition(0, -mult); break;
				case sf::Keyboard::S: g.ChangePosition(0,  mult); break;
				case sf::Keyboard::A: g.ChangePosition(-mult, 0); break;
				case sf::Keyboard::D: g.ChangePosition( mult, 0); break;

				case sf::Keyboard::Z: g.ChangeIterations(static_cast<int>( mult)); break;
				case sf::Keyboard::X: g.ChangeIterations(static_cast<int>(-mult)); break;

				case sf::Keyboard::Q: g.ChangeZoom(static_cast<float>(     2 * mult / 100)); break;
				case sf::Keyboard::E: g.ChangeZoom(static_cast<float>(1. / 2 * mult / 100)); break;

				case sf::Keyboard::J:
					g.SetJulia(event.key.shift, event.key.control, event.key.alt);
					break;
				case sf::Keyboard::M:
					g.SetMandelbrot(event.key.shift, event.key.control, event.key.alt);
					break;

				case sf::Keyboard::U:
					g.ToggleAutoupdate();
					break;

				case sf::Keyboard::G:
					g.ToggleGrid();
					break;

				case sf::Keyboard::Space:
					if (event.key.control)
						g.SetChanged();
					g.SetUpdate();
					break;

				case sf::Keyboard::R:
					if (event.key.control)
						g.Reset(true);
					else if (event.key.shift)
						g.Reset(false);
					break;

				default:
					break;
				}
			}
		}
		g.Update();
		info = g.ShowInfo(shr_fc);
		if (!info.empty()) {
			system("cls");
			std::cout << info << std::endl;
		}

		window.clear(sf::Color::White);
		window.draw(spr_mand);
		g.Draw(window);
		window.display();
	}
}
