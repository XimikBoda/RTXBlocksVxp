#pragma once

#define M_PI       3.14159265358979323846
const float pi = M_PI;

typedef int int_fixed;

#define FRACT_BITS 9
#define FIXED_POINT_ONE (1 << FRACT_BITS)
#define MAKE_INT_FIXED(x) ((x) << FRACT_BITS)
#define MAKE_FLOAT_FIXED(x) ((int_fixed)((x) * FIXED_POINT_ONE))
#define MAKE_FIXED_INT(x) ((x) >> FRACT_BITS)
#define MAKE_FIXED_FLOAT(x) (((float)(x)) / FIXED_POINT_ONE)

#define FIXED_MULT(x, y) ((x)*(y) >> FRACT_BITS)
#define FIXED_DIV(x, y) (((x)<<FRACT_BITS) / (y))

#define VM_COLOR_888_TO_565(r, g, b)	(((r & 0xf8) + ((g & 0xe0) >> 5)) << 8) + ((g & 0x1c) << 3) + (b >> 3)
//#define VM_COLOR_RGB565_TO_RGB32(color16)	((color16 & 0x001F) << 27) + ((color16 & 0x07E0) << 13) + ((color16 & 0xF800))
#define VM_COLOR_565_TO_888(color16)	((color16 & 0x001F) << 19) + ((color16 & 0x07E0) << 5) + ((color16 & 0xF800) >> 8)