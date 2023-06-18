#include "World.h"//
#include "BlockPalette.h"
#include "Player.h"
#ifndef MRE
#include <corecrt_malloc.h>
#include <imgui.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#else
#include "vmstdlib.h"
#include <cstdlib>
#include "string.h"
#endif // !MRE
unsigned char* world = 0;

int center_chunk_x = 0;
int center_chunk_z = 0;
int center_chunk_y = 0;

int start_chunk_x = 0;
int start_chunk_z = 0;
int start_chunk_y = 0;

extern Player_s player;

namespace World {
	void init(){
		world = (unsigned char*)malloc((WORLD__R_S_X*WORLD__R_S_Z*WORLD__R_S_Y)*2);
		for (int i = 0; i < WORLD__R_S_X * WORLD__R_S_Z * WORLD__R_S_Y; ++i)
			world[i] = 0;
	}

	void clear_chunck(int cx, int cy, int cz) {
		cx *= 16;
		cy *= 16;
		cz *= 16;
		for (int x = 0; x < 16/4; ++x)
			for (int z = 0; z < 16; ++z)
				for (int y = 0; y < 16; ++y)
					*((unsigned int*)&world[(cx + x*4) + ((cz + z) << WORLD_Z_SH) + ((cy + y) << WORLD_Y_SH)]) = 0;
	}

	void copy_chunck(int x1, int y1, int z1, int x2, int y2, int z2) { //from to
		x1 *= 16;
		y1 *= 16;
		z1 *= 16;
		x2 *= 16;
		y2 *= 16;
		z2 *= 16;
		for (int x = 0; x < 16 / 4; ++x)
			for (int z = 0; z < 16; ++z)
				for (int y = 0; y < 16; ++y)
					*((unsigned int*)&world[(x2 + x * 4) + ((z2 + z) << WORLD_Z_SH) + ((y2 + y) << WORLD_Y_SH)]) = *((unsigned int*)&world[(x1 + x * 4) + ((z1 + z) << WORLD_Z_SH) + ((y1 + y) << WORLD_Y_SH)]);
	}



	void set_center_chunk(int x, int z, int y) {
		int delta_x = x - center_chunk_x;
		int delta_y = y - center_chunk_y;
		int delta_z = z - center_chunk_z;

		if (!(delta_x || delta_y || delta_z))
			return;
		//delta_x *= -1;
		if (delta_x >= WORLD__RC_S_X || delta_x <= -WORLD__RC_S_X)
			memset(world,0, WORLD__R_S_X* WORLD__R_S_Z* WORLD__R_S_Y);
		else if (delta_x > 0)
			for (int yy = 0; yy < WORLD__RC_S_Y; ++yy)
				for (int zz = 0; zz < WORLD__RC_S_Z; ++zz) {
					for (int xx = 0; xx < WORLD__RC_S_X - delta_x; ++xx)
						copy_chunck(xx + delta_x, yy, zz, xx, yy, zz);
					for (int xx = WORLD__RC_S_X - delta_x; xx < WORLD__RC_S_X; ++xx)
						clear_chunck(xx, yy, zz);
				}
		else if(delta_x<0)
			for (int yy = 0; yy < WORLD__RC_S_Y; ++yy)
				for (int zz = 0; zz < WORLD__RC_S_Z; ++zz) {
					for (int xx = 0; xx < WORLD__RC_S_X + delta_x; ++xx)
						copy_chunck(WORLD__RC_S_X-1-(xx - delta_x), yy, zz, WORLD__RC_S_X - 1 - xx, yy, zz);
					for (int xx = WORLD__RC_S_X + delta_x; xx < WORLD__RC_S_X; ++xx)
						clear_chunck(WORLD__RC_S_X - 1 - xx, yy, zz);
				}

		if (delta_z >= WORLD__RC_S_Z || delta_z <= -WORLD__RC_S_Z)
			memset(world, 0, WORLD__R_S_X * WORLD__R_S_Z * WORLD__R_S_Y);
		else if (delta_z > 0)
			for (int yy = 0; yy < WORLD__RC_S_Y; ++yy)
				for (int xx = 0; xx < WORLD__RC_S_X; ++xx) {
					for (int zz = 0; zz < WORLD__RC_S_Z - delta_z; ++zz)
						copy_chunck(xx, yy, zz + delta_z, xx, yy, zz);
					for (int zz = WORLD__RC_S_Z - delta_z; zz < WORLD__RC_S_Z; ++zz)
						clear_chunck(xx, yy, zz);
				}
		else if (delta_z < 0)
			for (int yy = 0; yy < WORLD__RC_S_Y; ++yy)
				for (int xx = 0; xx < WORLD__RC_S_X; ++xx) {
					for (int zz = 0; zz < WORLD__RC_S_Z + delta_z; ++zz)
						copy_chunck(xx, yy, WORLD__RC_S_Z - 1 - (zz - delta_z), xx, yy, WORLD__RC_S_Z - 1 - zz);
					for (int zz = WORLD__RC_S_Z + delta_z; zz < WORLD__RC_S_Z; ++zz)
						clear_chunck(xx, yy, WORLD__RC_S_Z - 1 - zz);
				}

		if (delta_y >= WORLD__RC_S_Y || delta_y <= -WORLD__RC_S_Y)
			memset(world, 0, WORLD__R_S_X * WORLD__R_S_Z * WORLD__R_S_Y);
		else if (delta_y > 0)
			for (int xx = 0; xx < WORLD__RC_S_X; ++xx)
				for (int zz = 0; zz < WORLD__RC_S_Z; ++zz) {
					for (int yy = 0; yy < WORLD__RC_S_Y - delta_y; ++yy)
						copy_chunck(xx, yy + delta_y, zz, xx, yy, zz);
					for (int yy = WORLD__RC_S_Y - delta_y; yy < WORLD__RC_S_Y; ++yy)
						clear_chunck(xx, yy, zz);
				}
		else if (delta_y < 0)
			for (int xx = 0; xx < WORLD__RC_S_X; ++xx)
				for (int zz = 0; zz < WORLD__RC_S_Z; ++zz) {
					for (int yy = 0; yy < WORLD__RC_S_Y + delta_y; ++yy)
						copy_chunck(xx, WORLD__RC_S_Y - 1 - (yy - delta_y), zz, xx, WORLD__RC_S_Y - 1 - yy, zz);
					for (int yy = WORLD__RC_S_Y + delta_y; yy < WORLD__RC_S_Y; ++yy)
						clear_chunck(xx, WORLD__RC_S_Y - 1 - yy, zz);
				}

		player.x -= delta_x * 16;
		player.z -= delta_z * 16;
		player.y -= delta_y * 16;

		center_chunk_x = x;
		center_chunk_z = z;
		center_chunk_y = y;



		start_chunk_x = center_chunk_x - (WORLD__RC_S_X - 1) / 2;
		start_chunk_z = center_chunk_z - (WORLD__RC_S_Z - 1) / 2;
		start_chunk_y = center_chunk_y - (WORLD__RC_S_Y - 1) / 2;

	}


	void load_chunck_section(int cx, int cy, int cz, unsigned short *a) {
		//printf("cx = %d, cy = %d, cz = %d\n", cx, cy,cz);
		if (cx < start_chunk_x || cy < start_chunk_y || cz < start_chunk_z ||
			cx >= start_chunk_x + WORLD__RC_S_X || cy >= start_chunk_y + WORLD__RC_S_Y ||
			cz >= start_chunk_z + WORLD__RC_S_Z)
			return;


		cx -= start_chunk_x;
		cy -= start_chunk_y;
		cz -= start_chunk_z;

		int count = 0;
		for (int i = 0; i < 16 * 16 * 16; ++i)
			if (a[i])
				count++;

		//printf("cx = %d, cy = %d, cz = %d ==========  %d\n\n", cx, cy, cz, count);

		cx *= 16;
		cy *= 16;
		cz *= 16;

		for (int x = 0; x < 16; ++x)
			for (int z = 0; z < 16; ++z)
				for (int y = 0; y < 16; ++y)
					/*if(cz<4)
						world[(cx + x) + ((cz + z) << 7) + ((cy + y) << 14)] = 0xFF & a[x + z * 16 + (15 - y) * 16 * 16];
					else*/
						world[(cx + x) + ((cz + z) << WORLD_Z_SH) + ((cy + y) << WORLD_Y_SH)] = BlockPalette::get_block_id(a[x + z * 16 + y * 16 * 16]);
		
	}

	void set_block(int x, int y, int z, unsigned short id) {
		x -= start_chunk_x*16;
		y -= start_chunk_y*16;
		z -= start_chunk_z*16;
		if (x < 0 || y < 0 || z < 0 ||
			x >= WORLD__R_S_X || y >= WORLD__R_S_X || z >= WORLD__R_S_X)
			return;
		world[(x)+((z) << WORLD_Z_SH) + ((y) << WORLD_Y_SH)] = BlockPalette::get_block_id(id);
	}

	unsigned short get_block_l(int x, int y, int z) {
		if (x < 0 || y < 0 || z < 0 ||
			x >= WORLD__R_S_X || y >= WORLD__R_S_X || z >= WORLD__R_S_X)
			return 511;
		return BlockPalette::get_big_block_id(world[(x)+((z) << WORLD_Z_SH) + ((y) << WORLD_Y_SH)]);
	}

	unsigned short get_block(int x, int y, int z) {
		x -= start_chunk_x * 16;
		y -= start_chunk_y * 16;
		z -= start_chunk_z * 16;
		if (x < 0 || y < 0 || z < 0 ||
			x >= WORLD__R_S_X || y >= WORLD__R_S_X || z >= WORLD__R_S_X)
			return 511;
		return BlockPalette::get_big_block_id(world[(x)+((z) << WORLD_Z_SH) + ((y) << WORLD_Y_SH)]);
	}

	int level=0;

#ifndef MRE
	void ImGui_draw() {
		if (ImGui::Begin("World")) {
			ImGui::Text("Center_chunk: %d, %d, %d", center_chunk_x, center_chunk_y, center_chunk_z);
		}
		ImGui::End();
	}
	void test_draw(sf::Texture& temp_preview, sf::RenderWindow& window) {
		for (int x = 0; x < WORLD__R_S_X; ++x)
			for (int z = 0; z < WORLD__R_S_Z; ++z)
				for (int y = WORLD__R_S_Y-1; y>=1; --y)
					if (world[x + (z << WORLD_Z_SH) + (y << WORLD_Y_SH)]) {
						int id = world[x + (z << WORLD_Z_SH) + (y << WORLD_Y_SH)];
						sf::Sprite sp(temp_preview);
						sp.setPosition(x*16, z*16);
						sp.setTextureRect(sf::IntRect((id%16)*16,(id/16)*16,16,16));
						//sp.scale(0.5, 0.5);
						window.draw(sp);
						break;
					}

	}
#endif
}
