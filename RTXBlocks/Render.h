#pragma once
#ifndef MRE
#include <SFML/Graphics/Texture.hpp>
#endif // !MRE

const int s_w = 240, s_h = 320;

namespace Render
{
	void init();
	void clear_to_transparent_color(unsigned short* buf);
	void draw_text_white(unsigned short* buf, int x, int y, const char* str);
#ifndef MRE
	void render(sf::Texture& tex);
	void render2(sf::Texture& tex);
#endif // !MRE
	void main_render();
};

