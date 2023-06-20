int_fixed vvxf = FIXED_MULT(nwf, vxf), vvzf = FIXED_MULT(nhf, vyf);
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