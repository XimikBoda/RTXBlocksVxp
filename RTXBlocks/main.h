#pragma once

#define M_PI       3.14159265358979323846
const float pi = M_PI;

const int s_w = 240, s_h = 320;


#define VM_COLOR_888_TO_565(r, g, b)	(((r & 0xf8) + ((g & 0xe0) >> 5)) << 8) + ((g & 0x1c) << 3) + (b >> 3)
//#define VM_COLOR_RGB565_TO_RGB32(color16)	((color16 & 0x001F) << 27) + ((color16 & 0x07E0) << 13) + ((color16 & 0xF800))
#define VM_COLOR_565_TO_888(color16)	((color16 & 0x001F) << 19) + ((color16 & 0x07E0) << 5) + ((color16 & 0xF800) >> 8)

const unsigned short tr_color = VM_COLOR_888_TO_565(255, 0, 255);

typedef int int_fixed;

namespace Main {
	void init_all();
	void init_all2();
	void deinit_all();

	void game_loop(int d_time);

	void handle_keyevt(int event, int keycode);
	void handle_penevt(int event, int x, int y);
}

enum GameState {
	UI,
	PlayS,
	ChatS
};

#ifndef MRE
#define vm_malloc malloc
#define vm_realloc realloc
#define vm_free free
#endif // !MRE


#define FRACT_BITS 9
#define FIXED_POINT_ONE (1 << FRACT_BITS)
#define MAKE_INT_FIXED(x) ((x) << FRACT_BITS)
#define MAKE_FLOAT_FIXED(x) ((int_fixed)((x) * FIXED_POINT_ONE))
#define MAKE_FIXED_INT(x) ((x) >> FRACT_BITS)
#define MAKE_FIXED_FLOAT(x) (((float)(x)) / FIXED_POINT_ONE)

#define FIXED_MULT(x, y) ((x)*(y) >> FRACT_BITS)
#define FIXED_DIV(x, y) (((x)<<FRACT_BITS) / (y))

#define FIXED_POINT_ONE_FR(fr) (1 << (fr))
#define MAKE_INT_FIXED_FR(fr, x) ((x) << (fr))
#define MAKE_FLOAT_FIXED_FR(fr, x) ((int_fixed)((x) * FIXED_POINT_ONE_FR(fr)))
#define MAKE_FIXED_INT_FR(fr, x) ((x) >> (fr))
#define MAKE_FIXED_FLOAT_FR(fr, x) (((float)(x)) / FIXED_POINT_ONE_FR(fr))
#define MAKE_FIXED_FIXED_FR(fr1, fr2, x) ((x) << ((fr2)-(fr1)))

#define FIXED_MULT_FR(fr, x, y) ((x)*(y) >> (fr))
#define FIXED_DIV_FR(fr, x, y) (((x)<<(fr)) / (y))
