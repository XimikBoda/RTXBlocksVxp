#pragma once
#include <SFML/Graphics/Texture.hpp>

const int s_w = 240*2, s_h = 320*2;

namespace Render
{
	void init();
	void render(sf::Texture& tex);
	void render2(sf::Texture& tex);
	void main_render();
};

