#include "main.h"

#define WORLD__R_S_X 128
#define WORLD__R_S_Y 128
#define WORLD__R_S_Z 32

struct Camera {
	float x, y, z;
	float a, b, c;
	float f;
};