#pragma once

/* 
 * std::mutex sh_mut_;
 * struct matrix {
 * std::vector<int> row;
 * bool done;
 * std::mutex mut;
 * }
 */

#include <SFML/Graphics.hpp>

#include "Workers.h"
#include "FractalSet.h"
#include "SharedFractalColor.h"

class FractalSetGraphic
{
public:

	FractalSetGraphic(short w, short h, std::shared_ptr<SharedFractalColor> color_obj, int workers_num = 8);
	~FractalSetGraphic();

	void ApplyTexture(sf::Texture& tex);

	void Generate(FractalSet* fset, FractalSet& obj_arg);

	void SetPixels(short y) const;

private:
	inline static void SetPixelColor_(uint8_t* p, uint32_t color);
	inline static void SetPixelColor_(uint8_t* p, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	
	short width_;
	short height_;
	uint8_t* pixels_;
	int* iters_;
	std::vector<std::vector<int>> v_iters_;

	Workers workers_;
	std::shared_ptr<const SharedFractalColor> fcolor_;
};

