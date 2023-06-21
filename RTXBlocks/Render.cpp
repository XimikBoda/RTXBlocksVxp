#include "Render.h"
#include "World.h"
#include "main.h"
#include <cmath>
#include "Protocol.h"
#include "Player.h"
#include "Profont6x11.h"
#ifndef MRE
#include <imgui.h>
sf::Uint8* pixels;
#else
#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
extern VMUINT16* layer_bufs[2];
#endif // !MRE

#define VM_COLOR_888_TO_565(r, g, b)	(((r & 0xf8) + ((g & 0xe0) >> 5)) << 8) + ((g & 0x1c) << 3) + (b >> 3)
const unsigned short tr_color = VM_COLOR_888_TO_565(255, 0, 255);

extern int render_c;

unsigned short* main_canvas_buff;
int_fixed* main_deep_buff;

unsigned short* main_canvas_buff2;
int_fixed* main_deep_buff2;

unsigned short* second_canvas_buff;
float* second_deep_buff;

extern unsigned short* blocks;

//temp
struct Camera {
	float x = 7, y = 2, z = 3;
	float a = pi, b = pi / 2, c = pi / 2;
	float f = 30;
};

Camera camera;

extern Player_s player;

extern unsigned char* world;

float sqr(float x) {
	return x * x;
}

int_fixed sqrf(int_fixed x) {
	return (((unsigned int)x & 0x7FFFFFFF) * ((unsigned int)x & 0x7FFFFFFF)) >> FRACT_BITS;
	//return (x * x) >> FRACT_BITS;

}

float drob(float a) {
	if (a >= 0)
		return a - int(a);
	else
		return (1.f + a - int(a));
}

int move = 20;

extern int start_chunk_x;
extern int start_chunk_z;
extern int start_chunk_y;

namespace Render {
	void init() {
#ifndef MRE
		main_canvas_buff = (unsigned short*)malloc(s_w * s_h * 2);
		main_deep_buff = (int_fixed*)malloc(s_w * s_h * 4);
		main_canvas_buff2 = (unsigned short*)malloc(s_w * s_h * 2);
		main_deep_buff2 = (int_fixed*)malloc(s_w * s_h * 4);
		second_canvas_buff = (unsigned short*)malloc(s_w * s_h * 2);
		second_deep_buff = (float*)malloc(s_w * s_h * 4);
		pixels = (sf::Uint8*)malloc(s_w * s_h * 4);
#else
		main_canvas_buff = (unsigned short*)(layer_bufs[0]);
		main_deep_buff = (int_fixed*)vm_malloc(s_w * s_h * 4);
#endif // !MRE

	}

	void clear_to_transparent_color(unsigned short* buf) {
		const unsigned int word = (tr_color << 16) | (tr_color);
		unsigned int* buf_i = (unsigned int*)buf;
		for (int i = 0; i < s_w * s_h / 2; ++i)
			buf_i[i] = word;
	}

	void draw_text_white(unsigned short *buf, int x, int y, const char *str) {
		for (int ci = 0; *str; ++ci, ++str) {
			const unsigned char* font_ch = ProFont6x11 + 5 + 12 * (*str) + 1;
			//unsigned short textcolor = main_text[y][x].textcolor, backcolor = main_text[y][x].backcolor;
			//if (main_text[y][x].flgs & 1)
			//	textcolor = ~textcolor, backcolor = ~backcolor;

			int t_char_width = pro_char_width;
			if (x + ci * pro_char_width + pro_char_width > s_w) {
				t_char_width = s_w - x + ci * pro_char_width;
				if (t_char_width <= 0)
					break;
			}
			for (int i = 0; i < pro_char_height; ++i) {
				unsigned short* scr_buf = (unsigned short*)buf + x + ci * pro_char_width + (y + i) * s_w;
				for (int j = 0; j < t_char_width; ++j)
					if(((*font_ch) >> j) &1)
						scr_buf[j] = 0xFFFF;
				//scr_buf[j] = ((((*font_ch) >> j) & 1) ? textcolor : backcolor);
				++font_ch;
			}
		}
	}

	bool pl_c = true;
#ifndef MRE
	void render(sf::Texture& tex) {
		sf::Image im;
		long* p = (long*)pixels;
		for (int i = 0; i < s_w * s_h; ++i)
			p[i] = 0xFF000000 | VM_COLOR_565_TO_888(main_canvas_buff[i]);
		im.create(s_w, s_h, pixels);
		tex.loadFromImage(im);
	}

	void render2(sf::Texture& tex) {
		sf::Image im;
		long* p = (long*)pixels;
		for (int i = 0; i < s_w * s_h; ++i)
			p[i] = 0xFF000000 | VM_COLOR_565_TO_888(main_canvas_buff2[i]);
		im.create(s_w, s_h, pixels);
		tex.loadFromImage(im);
	}


	void main_render() {
		if (pl_c) {

			camera.x = player.x;// -start_chunk_x * 16;
			camera.y = player.y + player.get_h();// -start_chunk_y * 16;
			camera.z = player.z;// -start_chunk_z * 16;
			camera.a = player.a + 0.1;
			camera.b = player.b + 0.1;
			//camera.c = player.c;
		}
		{
			{
				ImGui::Begin("Camera");
				ImGui::SliderFloat3("Pos", &camera.x, 0, 128);
				ImGui::SliderFloat("A", &camera.a, -4 * pi, 4 * pi);
				ImGui::SliderFloat("B", &camera.b, 0, pi);
				ImGui::SliderFloat("C", &camera.c, 0, pi * 2);
				ImGui::SliderFloat("F", &camera.f, 0, 100);
				ImGui::Checkbox("Auto camera", &pl_c);
				ImGui::End();
			}
		}
		{
			float L = 3;
			float nw = L * tan(camera.c / 2.f), nh = L * tan(camera.c * 2.f / 3.f);
			float a = fmod(camera.a, 2 * pi), b = camera.b;
			if (a < 0)
				a += pi * 2;
			int a_i = a / pi * 2;
			float nx1 = cos(a) * sin(b), nz1 = sin(a) * sin(b), ny1 = cos(b);
			float nx2 = sqrt(sqr(nz1) / (sqr(nz1) + sqr(nx1))), nz2 = sqrt(sqr(nx1) / (sqr(nz1) + sqr(nx1))), ny2 = 0;;
			if (a_i == 10 || a_i == 2 || a_i == 3)
				nx2 *= -1;
			if (a_i == 0 || a_i == 3)
				nz2 *= -1;
			float nx3 = nz1 * ny2 - ny1 * nz2, nz3 = -(nx1 * ny2 - ny1 * nx2), ny3 = nx1 * nz2 - nz1 * nx2;
			//float cx = camera.x, cz = camera.z, cy = camera.y;
			nx1 *= L, nz1 *= L, ny1 *= L;

			//Vector_data vector_data;
			const int imw = s_w, imh = s_h;

			int_fixed cxf = MAKE_FLOAT_FIXED(camera.x), czf = MAKE_FLOAT_FIXED(camera.z), cyf = MAKE_FLOAT_FIXED(camera.y);
			int_fixed nwf = MAKE_FLOAT_FIXED(-nw / float(imw / 2)), nhf = MAKE_FLOAT_FIXED(nh / float(imh / 2)), Lf = MAKE_FLOAT_FIXED(L);
			int_fixed nx1f = MAKE_FLOAT_FIXED(nx1), nz1f = MAKE_FLOAT_FIXED(nz1), ny1f = MAKE_FLOAT_FIXED(ny1);
			int_fixed nx2f = MAKE_FLOAT_FIXED(nx2), nz2f = MAKE_FLOAT_FIXED(nz2), ny2f = MAKE_FLOAT_FIXED(ny2);
			int_fixed nx3f = MAKE_FLOAT_FIXED(nx3), nz3f = MAKE_FLOAT_FIXED(nz3), ny3f = MAKE_FLOAT_FIXED(ny3);
			int_fixed cam_f = MAKE_FLOAT_FIXED(camera.f / 255.f);

			//printf("%d %d %d\n", nwf, nhf, Lf);

			nhf = -nwf;
			unsigned short* buf_i = &main_canvas_buff[0];
			int_fixed* hbuf_i = &main_deep_buff[0];
#pragma omp parallel for schedule(dynamic,imw/2)
			for (int i = 0; i < imw * imh; ++i) {
				int_fixed vxf = MAKE_INT_FIXED(-imw / 2 + i % imw), vzf = MAKE_INT_FIXED(-imh / 2 + i / imw);
				int_fixed vvxf = FIXED_MULT(nwf, vxf), vvzf = FIXED_MULT(nhf, vzf);
				int_fixed nxf = nx1f + FIXED_MULT(vvxf, nx2f) + FIXED_MULT(vvzf, nx3f);
				int_fixed nzf = nz1f + FIXED_MULT(vvxf, nz2f) + FIXED_MULT(vvzf, nz3f);
				int_fixed nyf = ny1f + FIXED_MULT(vvxf, ny2f) + FIXED_MULT(vvzf, ny3f);
				unsigned short c = VM_COLOR_888_TO_565(11, 98, 164), c2 = 0;
				int_fixed deep = MAKE_INT_FIXED(255 * 255);
				int_fixed l = 0;
				{
					unsigned char bl = 0;
					if (nxf < 0) {
						int_fixed nx_inv = FIXED_DIV(MAKE_INT_FIXED(1), nxf);
						for (int x = ((cxf < MAKE_INT_FIXED(WORLD__R_S_X)) ? MAKE_FIXED_INT(cxf) : WORLD__R_S_X); x > 0; --x) {
							int_fixed div = FIXED_MULT((MAKE_INT_FIXED(x) - cxf), nx_inv);
							int_fixed z = FIXED_MULT(div, nzf) + czf;
							int_fixed y = FIXED_MULT(div, nyf) + cyf;
							if (z >= MAKE_INT_FIXED(0)) {
								if (z < MAKE_INT_FIXED(WORLD__R_S_Z)) {
									if (y >= MAKE_INT_FIXED(0)) {
										if (y < MAKE_INT_FIXED(WORLD__R_S_Y)) {
											if (bl = world[(x - 1) + (MAKE_FIXED_INT(z) << WORLD_Z_SH) + (MAKE_FIXED_INT(y) << WORLD_Y_SH)]) {
												if ((l = sqrf(cxf - MAKE_INT_FIXED(x)) + sqrf(czf - z) + sqrf(cyf - y)) < deep) {
													int drx = MAKE_FIXED_INT((z & (FIXED_POINT_ONE - 1)) << 4);
													int drz = MAKE_FIXED_INT((y & (FIXED_POINT_ONE - 1)) << 4);
													if ((c2 = blocks[(drz << 4) + (bl << 8) + drx]) == VM_COLOR_888_TO_565(255, 0, 255))
														continue;
													c = c2;
													deep = l;
												}
												break;
											}
										}
										else if (nyf > 0) break;
									}
									else if (nyf < 0) break;
								}
								else if (nzf > 0) break;
							}
							else if (nzf < 0) break;
						}
					}
					if (nzf < 0) {
						int_fixed nz_inv = FIXED_DIV(MAKE_INT_FIXED(1), nzf);
						for (int z = ((czf < MAKE_INT_FIXED(WORLD__R_S_Z)) ? MAKE_FIXED_INT(czf) : WORLD__R_S_Z); z > 0; --z) {
							int_fixed div = FIXED_MULT((MAKE_INT_FIXED(z) - czf), nz_inv);
							int_fixed x = FIXED_MULT(div, nxf) + cxf;
							int_fixed y = FIXED_MULT(div, nyf) + cyf;
							if (x >= MAKE_INT_FIXED(0)) {
								if (x < MAKE_INT_FIXED(WORLD__R_S_X)) {
									if (y >= MAKE_INT_FIXED(0)) {
										if (y < MAKE_INT_FIXED(WORLD__R_S_Y)) {
											if (bl = world[MAKE_FIXED_INT(x) + ((z - 1) << WORLD_Z_SH) + (MAKE_FIXED_INT(y) << WORLD_Y_SH)]) {
												if ((l = sqrf(cxf - x) + sqrf(czf - MAKE_INT_FIXED(z)) + sqrf(cyf - y)) < deep) {
													int drx = MAKE_FIXED_INT((x & (FIXED_POINT_ONE - 1)) << 4);
													int drz = MAKE_FIXED_INT((y & (FIXED_POINT_ONE - 1)) << 4);
													if ((c2 = blocks[(drz << 4) + (bl << 8) + drx]) == VM_COLOR_888_TO_565(255, 0, 255))
														continue;
													c = c2;
													deep = l;
												}
												break;
											}
										}
										else if (nyf > 0) break;
									}
									else if (nyf < 0) break;
								}
								else if (nxf > 0) break;
							}
							else if (nxf < 0) break;
						}
					}
					if (nyf < 0) {
						int_fixed ny_inv = FIXED_DIV(MAKE_INT_FIXED(1), nyf);
						for (int y = ((cyf < MAKE_INT_FIXED(WORLD__R_S_Y)) ? MAKE_FIXED_INT(cyf) : WORLD__R_S_Y); y > 0; --y) {
							int_fixed div = FIXED_MULT((MAKE_INT_FIXED(y) - cyf), ny_inv);
							int_fixed x = FIXED_MULT(div, nxf) + cxf;
							int_fixed z = FIXED_MULT(div, nzf) + czf;
							if (x >= MAKE_INT_FIXED(0)) {
								if (x < MAKE_INT_FIXED(WORLD__R_S_X)) {
									if (z >= MAKE_INT_FIXED(0)) {
										if (z < MAKE_INT_FIXED(WORLD__R_S_Z)) {
											if (bl = world[MAKE_FIXED_INT(x) + (MAKE_FIXED_INT(z) << WORLD_Z_SH) + ((y - 1) << WORLD_Y_SH)]) {
												if ((l = sqrf(cxf - x) + sqrf(czf - z) + sqrf(cyf - MAKE_INT_FIXED(y))) < deep) {
													int drx = MAKE_FIXED_INT((x & (FIXED_POINT_ONE - 1)) << 4);
													int drz = MAKE_FIXED_INT((z & (FIXED_POINT_ONE - 1)) << 4);
													if ((c2 = blocks[(drz << 4) + (bl << 8) + drx]) == VM_COLOR_888_TO_565(255, 0, 255))
														continue;
													c = c2;
													deep = l;
												}
												break;
											}
										}
										else if (nzf > 0) break;
									}
									else if (nzf < 0) break;
								}
								else if (nxf > 0) break;
							}
							else if (nxf < 0) break;
						}
					}
					if (nxf > 0) {

						int_fixed nx_inv = FIXED_DIV(MAKE_INT_FIXED(1), nxf); \
							for (int x = ((cxf + MAKE_INT_FIXED(1) > 0) ? MAKE_FIXED_INT(cxf) + 1 : 0); x < WORLD__R_S_X; ++x) {
								int_fixed div = FIXED_MULT((MAKE_INT_FIXED(x) - cxf), nx_inv);
								int_fixed z = FIXED_MULT(div, nzf) + czf;
								int_fixed y = FIXED_MULT(div, nyf) + cyf;
								if (z >= MAKE_INT_FIXED(0)) {
									if (z < MAKE_INT_FIXED(WORLD__R_S_Z)) {
										if (y >= MAKE_INT_FIXED(0)) {
											if (y < MAKE_INT_FIXED(WORLD__R_S_Y)) {
												if (bl = world[x + (MAKE_FIXED_INT(z) << WORLD_Z_SH) + (MAKE_FIXED_INT(y) << WORLD_Y_SH)]) {
													if ((l = sqrf(cxf - MAKE_INT_FIXED(x)) + sqrf(czf - z) + sqrf(cyf - y)) < deep) {
														int drx = MAKE_FIXED_INT((z & (FIXED_POINT_ONE - 1)) << 4);
														int drz = MAKE_FIXED_INT((y & (FIXED_POINT_ONE - 1)) << 4);
														if ((c2 = blocks[(drz << 4) + (bl << 8) + drx]) == VM_COLOR_888_TO_565(255, 0, 255))
															continue;
														c = c2;
														deep = l;
													}
													break;
												}
											}
											else if (nyf > 0) break;
										}
										else if (nyf < 0) break;
									}
									else if (nzf > 0) break;
								}
								else if (nzf < 0) break;
							}
					}
					if (nzf > 0) {
						int_fixed nz_inv = FIXED_DIV(MAKE_INT_FIXED(1), nzf);
						for (int z = ((czf + MAKE_INT_FIXED(1) > 0) ? MAKE_FIXED_INT(czf) + 1 : 0); z < WORLD__R_S_Z; ++z) {
							int_fixed div = FIXED_MULT((MAKE_INT_FIXED(z) - czf), nz_inv);
							int_fixed x = FIXED_MULT(div, nxf) + cxf;
							int_fixed y = FIXED_MULT(div, nyf) + cyf;
							if (x >= MAKE_INT_FIXED(0)) {
								if (x < MAKE_INT_FIXED(WORLD__R_S_X)) {
									if (y >= MAKE_INT_FIXED(0)) {
										if (y < MAKE_INT_FIXED(WORLD__R_S_Y)) {
											if (bl = world[MAKE_FIXED_INT(x) + (z << WORLD_Z_SH) + (MAKE_FIXED_INT(y) << WORLD_Y_SH)]) {
												if ((l = sqrf(cxf - x) + sqrf(czf - MAKE_INT_FIXED(z)) + sqrf(cyf - y)) < deep) {
													int drx = MAKE_FIXED_INT((x & (FIXED_POINT_ONE - 1)) << 4);
													int drz = MAKE_FIXED_INT((y & (FIXED_POINT_ONE - 1)) << 4);
													if ((c2 = blocks[(drz << 4) + (bl << 8) + drx]) == VM_COLOR_888_TO_565(255, 0, 255))
														continue;
													c = c2;
													deep = l;
												}
												break;
											}
										}
										else if (nyf > 0) break;
									}
									else if (nyf < 0) break;
								}
								else if (nxf > 0) break;
							}
							else if (nxf < 0) break;
						}
					}
					if (nyf > 0) {
						int_fixed ny_inv = FIXED_DIV(MAKE_INT_FIXED(1), nyf);
						for (int y = ((cyf + MAKE_INT_FIXED(1) > 0) ? MAKE_FIXED_INT(cyf) + 1 : 0); y < WORLD__R_S_Y; ++y) {
							int_fixed div = FIXED_MULT((MAKE_INT_FIXED(y) - cyf), ny_inv);
							int_fixed x = FIXED_MULT(div, nxf) + cxf;
							int_fixed z = FIXED_MULT(div, nzf) + czf;
							if (x >= MAKE_INT_FIXED(0)) {
								if (x < MAKE_INT_FIXED(WORLD__R_S_X)) {
									if (z >= MAKE_INT_FIXED(0)) {
										if (z < MAKE_INT_FIXED(WORLD__R_S_Z)) {
											if (bl = world[MAKE_FIXED_INT(x) + (MAKE_FIXED_INT(z) << WORLD_Z_SH) + (y << WORLD_Y_SH)]) {
												if ((l = sqrf(cxf - x) + sqrf(czf - z) + sqrf(cyf - MAKE_INT_FIXED(y))) < deep) {
													int drx = MAKE_FIXED_INT((x & (FIXED_POINT_ONE - 1)) << 4);
													int drz = MAKE_FIXED_INT((z & (FIXED_POINT_ONE - 1)) << 4);
													if ((c2 = blocks[(drz << 4) + (bl << 8) + drx]) == VM_COLOR_888_TO_565(255, 0, 255))
														continue;
													c = c2;
													deep = l;
												}\
													break;
											}\
										}
										else if (nzf > 0) break;
									}
									else if (nzf < 0) break;
								}
								else if (nxf > 0) break;
							}
							else if (nxf < 0) break;
						}
					}
				}
				buf_i[i] = c;
				hbuf_i[i] = deep;
				{
					double d = deep;
					d=sqrt(d);
#define VM_COLOR_GET_RED(color16)		((color16 & 0xF800) >> 8)

#define VM_COLOR_GET_GREEN(color16)	    ((color16 & 0x07E0) >> 3)

#define VM_COLOR_GET_BLUE(color16)		((color16 & 0x001F) << 3)

#define VM_COLOR_888_TO_565(r, g, b)	(((r & 0xf8) + ((g & 0xe0) >> 5)) << 8) + ((g & 0x1c) << 3) + (b >> 3)
					unsigned char r = VM_COLOR_GET_RED(c);
					unsigned char g = VM_COLOR_GET_GREEN(c);
					unsigned char b = VM_COLOR_GET_BLUE(c);
					float c_m = 1.f - sqrt(deep)  * camera.f/ 128.f/128.f;
					if (c_m < 0)
						c_m = 0;
					else if (c_m > 1)
						c_m = 1;
					r *= c_m;
					g *= c_m;
					b *= c_m;
					buf_i[i] = VM_COLOR_888_TO_565(r, g, b);
				}
			}
		}
	}
#else
void main_render() {
	if (pl_c) {

		camera.x = player.x;// -start_chunk_x * 16;
		camera.y = player.y + player.get_h();// -start_chunk_y * 16;
		camera.z = player.z;// -start_chunk_z * 16;
		camera.a = player.a + 0.1;
		camera.b = player.b + 0.1;
		camera.c = pi / 2;
		camera.f = 30;
	}
	{
		float L = 3;
		float nw = L * tan(camera.c / 2.f), nh = L * tan(camera.c * 2.f / 3.f);
		float a = fmod(camera.a, 2 * pi), b = camera.b;
		if (a < 0)
			a += pi * 2;
		int a_i = a / pi * 2;
		float nx1 = cos(a) * sin(b), nz1 = sin(a) * sin(b), ny1 = cos(b);
		float nx2 = sqrt(sqr(nz1) / (sqr(nz1) + sqr(nx1))), nz2 = sqrt(sqr(nx1) / (sqr(nz1) + sqr(nx1))), ny2 = 0;;
		if (a_i == 10 || a_i == 2 || a_i == 3)
			nx2 *= -1;
		if (a_i == 0 || a_i == 3)
			nz2 *= -1;
		float nx3 = nz1 * ny2 - ny1 * nz2, nz3 = -(nx1 * ny2 - ny1 * nx2), ny3 = nx1 * nz2 - nz1 * nx2;
		//float cx = camera.x, cz = camera.z, cy = camera.y;
		nx1 *= L, nz1 *= L, ny1 *= L;

		//Vector_data vector_data;
		const int imw = s_w, imh = s_h;
		const int v_w = s_w, v_h = s_h;

		int_fixed cxf = MAKE_FLOAT_FIXED(camera.x), czf = MAKE_FLOAT_FIXED(camera.z), cyf = MAKE_FLOAT_FIXED(camera.y);
		int_fixed nwf = MAKE_FLOAT_FIXED(-nw / float(imw / 2)), nhf = MAKE_FLOAT_FIXED(nh / float(imh / 2)), Lf = MAKE_FLOAT_FIXED(L);
		int_fixed nx1f = MAKE_FLOAT_FIXED(nx1), nz1f = MAKE_FLOAT_FIXED(nz1), ny1f = MAKE_FLOAT_FIXED(ny1);
		int_fixed nx2f = MAKE_FLOAT_FIXED(nx2), nz2f = MAKE_FLOAT_FIXED(nz2), ny2f = MAKE_FLOAT_FIXED(ny2);
		int_fixed nx3f = MAKE_FLOAT_FIXED(nx3), nz3f = MAKE_FLOAT_FIXED(nz3), ny3f = MAKE_FLOAT_FIXED(ny3);
		int_fixed cam_f = MAKE_FLOAT_FIXED(camera.f / 255.f);

		//printf("%d %d %d\n", nwf, nhf, Lf);

		{
			if (render_c >= 0) {
				move = 0;
				render_c--;
			}
		}

		nhf = -nwf;
		unsigned short* buf_i = &main_canvas_buff[0] - 4;
		int_fixed* hbuf_i = &main_deep_buff[0] - 4;
		if (move <= 0) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 1), vyf = MAKE_INT_FIXED(-v_h / 2 + 1);;
			do {
#include"render_trace.h"
				* (buf_i + 3 + 240 * 3) = *(buf_i + 2 + 240 * 3) = *(buf_i + 1 + 240 * 3) = *(buf_i + 240 * 3)
					= *(buf_i + 3 + 240 * 2) = *(buf_i + 2 + 240 * 2) = *(buf_i + 1 + 240 * 2) = *(buf_i + 240 * 2)
					= *(buf_i + 3 + 240) = *(buf_i + 2 + 240) = *(buf_i + 1 + 240) = *(buf_i + 240)
					= *(buf_i + 3) = *(buf_i + 2) = *(buf_i + 1) = *(buf_i += 4) = c;
				*(hbuf_i + 3 + 240 * 3) = *(hbuf_i + 2 + 240 * 3) = *(hbuf_i + 1 + 240 * 3) = *(hbuf_i + 240 * 3)
					= *(hbuf_i + 3 + 240 * 2) = *(hbuf_i + 2 + 240 * 2) = *(hbuf_i + 1 + 240 * 2) = *(hbuf_i + 240 * 2)
					= *(hbuf_i + 3 + 240) = *(hbuf_i + 2 + 240) = *(hbuf_i + 1 + 240) = *(hbuf_i + 240)
					= *(hbuf_i + 3) = *(hbuf_i + 2) = *(hbuf_i + 1) = *(hbuf_i += 4) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 1)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 1);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 1) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 1) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 3), vyf = MAKE_INT_FIXED(-v_h / 2 + 3);;
			do {
#include"render_trace.h"
				* (buf_i + 3 + 240 * 3) = *(buf_i + 2 + 240 * 3) = *(buf_i + 1 + 240 * 3) = *(buf_i + 240 * 3)
					= *(buf_i + 3 + 240 * 2) = *(buf_i + 2 + 240 * 2) = *(buf_i + 1 + 240 * 2) = *((buf_i += 4) + 240 * 2) = c;
				*(hbuf_i + 3 + 240 * 3) = *(hbuf_i + 2 + 240 * 3) = *(hbuf_i + 1 + 240 * 3) = *(hbuf_i + 240 * 3)
					= *(hbuf_i + 3 + 240 * 2) = *(hbuf_i + 2 + 240 * 2) = *(hbuf_i + 1 + 240 * 2) = *((hbuf_i += 4) + 240 * 2) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 3)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 3);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 3) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 2) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 3), vyf = MAKE_INT_FIXED(-v_h / 2 + 1);;
			do {
#include"render_trace.h"
				* (buf_i + 3 + 240) = *(buf_i + 2 + 240)
					= *(buf_i + 3) = *((buf_i += 4) + 2) = c;
				*(hbuf_i + 3 + 240) = *(hbuf_i + 2)
					= *(hbuf_i + 3) = *((hbuf_i += 4) + 2) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 3)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 3);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 1) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 3) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 1), vyf = MAKE_INT_FIXED(-v_h / 2 + 3);;
			do {
#include"render_trace.h"
				* (buf_i + 1 + 240 * 3) = *(buf_i + 240 * 3)
					= *(buf_i + 1 + 240 * 2) = *((buf_i += 4) + 240 * 2) = c;
				*(hbuf_i + 1 + 240 * 3) = *(hbuf_i + 240 * 3)
					= *(hbuf_i + 1 + 240 * 2) = *((hbuf_i += 4) + 240 * 2) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 1)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 1);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 3) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 4) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 1), vyf = MAKE_INT_FIXED(-v_h / 2 + 0);;
			do {
#include"render_trace.h"
				* (buf_i + 1) = *((buf_i += 4)) = c;
				*(hbuf_i + 1) = *((hbuf_i += 4)) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 1)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 1);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 0) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 5) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 3), vyf = MAKE_INT_FIXED(-v_h / 2 + 2);;
			do {
#include"render_trace.h"
				* (buf_i + 3 + 240 * 2) = *((buf_i += 4) + 2 + 240 * 2) = c;
				*(hbuf_i + 3 + 240 * 2) = *((hbuf_i += 4) + 2 + 240 * 2) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 3)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 3);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 2) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 6) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 3), vyf = MAKE_INT_FIXED(-v_h / 2 + 0);;
			do {
#include"render_trace.h"
				* (buf_i + 3) = *((buf_i += 4) + 2) = c;
				*(hbuf_i + 3) = *((hbuf_i += 4) + 2) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 3)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 3);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 0) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 7) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 1), vyf = MAKE_INT_FIXED(-v_h / 2 + 2);;
			do {
#include"render_trace.h"
				* (buf_i + 1 + 240 * 2) = *((buf_i += 4) + 0 + 240 * 2) = c;
				*(hbuf_i + 1 + 240 * 2) = *((hbuf_i += 4) + 0 + 240 * 2) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 1)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 1);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 2) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 8) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 0), vyf = MAKE_INT_FIXED(-v_h / 2 + 1);;
			do {
#include"render_trace.h"
				* ((buf_i += 4) + 0 + 240 * 1) = c;
				*((hbuf_i += 4) + 0 + 240 * 1) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 0)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 0);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 1) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 9) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 2), vyf = MAKE_INT_FIXED(-v_h / 2 + 3);;
			do {
#include"render_trace.h"
				* ((buf_i += 4) + 2 + 240 * 3) = c;
				*((hbuf_i += 4) + 2 + 240 * 3) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 2)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 2);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 3) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 10) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 2), vyf = MAKE_INT_FIXED(-v_h / 2 + 1);;
			do {
#include"render_trace.h"
				* ((buf_i += 4) + 2 + 240 * 1) = c;
				*((hbuf_i += 4) + 2 + 240 * 1) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 2)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 2);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 1) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 11) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 0), vyf = MAKE_INT_FIXED(-v_h / 2 + 3);;
			do {
#include"render_trace.h"
				* ((buf_i += 4) + 0 + 240 * 3) = c;
				*((hbuf_i += 4) + 0 + 240 * 3) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 0)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 0);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 3) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 12) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 0), vyf = MAKE_INT_FIXED(-v_h / 2 + 0);;
			do {
#include"render_trace.h"
				* ((buf_i += 4) + 0 + 240 * 0) = c;
				*((hbuf_i += 4) + 0 + 240 * 0) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 0)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 0);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 0) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 13) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 2), vyf = MAKE_INT_FIXED(-v_h / 2 + 2);;
			do {
#include"render_trace.h"
				* ((buf_i += 4) + 2 + 240 * 2) = c;
				*((hbuf_i += 4) + 2 + 240 * 2) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 2)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 2);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 2) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 14) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 2), vyf = MAKE_INT_FIXED(-v_h / 2 + 0);;
			do {
#include"render_trace.h"
				* ((buf_i += 4) + 2 + 240 * 0) = c;
				*((hbuf_i += 4) + 2 + 240 * 0) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 2)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 2);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 0) + MAKE_INT_FIXED(320));
			move++;
		}
		else if (move == 15) {
			int_fixed vxf = MAKE_INT_FIXED(-v_w / 2 + 0), vyf = MAKE_INT_FIXED(-v_h / 2 + 2);;
			do {
#include"render_trace.h"
				* ((buf_i += 4) + 0 + 240 * 2) = c;
				*((hbuf_i += 4) + 0 + 240 * 2) = deep;

				if ((vxf += MAKE_INT_FIXED(4)) == MAKE_INT_FIXED(v_w / 2 + 0)) {
					vyf += MAKE_INT_FIXED(4);
					vxf = MAKE_INT_FIXED(-v_w / 2 + 0);
					buf_i += 240 * 3;
					hbuf_i += 240 * 3;
				}
			} while (vyf != MAKE_INT_FIXED(-v_h / 2 + 2) + MAKE_INT_FIXED(320));
			move++;
		}
		/*		buf_i[i] = c;
				hbuf_i[i] = deep;
				if(0){
					double d = deep;
					d=sqrt(d);
#define VM_COLOR_GET_RED(color16)		((color16 & 0xF800) >> 8)

#define VM_COLOR_GET_GREEN(color16)	    ((color16 & 0x07E0) >> 3)

#define VM_COLOR_GET_BLUE(color16)		((color16 & 0x001F) << 3)

#define VM_COLOR_888_TO_565(r, g, b)	(((r & 0xf8) + ((g & 0xe0) >> 5)) << 8) + ((g & 0x1c) << 3) + (b >> 3)
					unsigned char r = VM_COLOR_GET_RED(c);
					unsigned char g = VM_COLOR_GET_GREEN(c);
					unsigned char b = VM_COLOR_GET_BLUE(c);
					float c_m = 1.f - sqrt(deep)  * camera.f/ 128.f/128.f;
					if (c_m < 0)
						c_m = 0;
					else if (c_m > 1)
						c_m = 1;
					r *= c_m;
					g *= c_m;
					b *= c_m;
					buf_i[i] = VM_COLOR_888_TO_565(r, g, b);
				}*/
				//}
	}
	}
#endif // !MRE

}