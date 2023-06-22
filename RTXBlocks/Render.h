#pragma once
#include "main.h"
#ifndef MRE
#include <SFML/Graphics/Texture.hpp>
#endif // !MRE

const int s_w = 240, s_h = 320;



namespace Render
{
	struct vector3f {
		float x = 0, y = 0, z = 0;
	};
	struct vertex2fd {
		float x = 0, y = 0;
		int_fixed d = 0;
	};
	struct poligone2f {
		vertex2fd v[3];
	};
	typedef vector3f vertex3f;
	/*struct vertex3f {
		float x = 0, y = 0, z = 0;
	};*/
	struct poligone3f {
		vertex3f v[3];
	};
	struct camera_cache {
		vector3f pos;
		vector3f cn;
		vector3f cnw;
		vector3f cnh;
		float nw, nh, L;

	};
	

	void init();
	void clear_to_transparent_color(unsigned short* buf);
	void draw_text_white(unsigned short* buf, int x, int y, const char* str);
	void texture_triangle_rasterezation(unsigned short* scr_buf, unsigned short* texture_buf, int t_w, float x1, float y1, float x2, float y2, float x3, float y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3);
#ifndef MRE
	void render(sf::Texture& tex);
	void render2(sf::Texture& tex);
#endif // !MRE
	void main_render();
};

