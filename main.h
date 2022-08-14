#pragma once
#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "ResID.h"
#include "vm4res.h"	
#include "vmres.h"
#include "vmtimer.h"
#include "string.h"

#include <cstdlib>

#include <cmath>

#define M_PI       3.14159265358979323846

const unsigned short tr_color = VM_COLOR_888_TO_565(255, 0, 255);

void show_error_and_exit(const char *text);

#ifdef WIN32
#define my_printf(...) printf(__VA_ARGS__)
#else
#define my_printf(...)
#endif

#define FRACT_BITS 9
#define FIXED_POINT_ONE (1 << FRACT_BITS)
#define MAKE_INT_FIXED(x) ((x) << FRACT_BITS)
#define MAKE_FLOAT_FIXED(x) ((int_fixed)((x) * FIXED_POINT_ONE))
#define MAKE_FIXED_INT(x) ((x) >> FRACT_BITS)
#define MAKE_FIXED_FLOAT(x) (((float)(x)) / FIXED_POINT_ONE)

#define FIXED_MULT(x, y) ((x)*(y) >> FRACT_BITS)
#define FIXED_DIV(x, y) (((x)<<FRACT_BITS) / (y))