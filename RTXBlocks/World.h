#pragma once
#ifndef MRE
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#endif // !MRE

#define WORLD_X_M 3
#define WORLD_Z_M 3

#define WORLD__RC_S_X (1<<WORLD_X_M)
#define WORLD__RC_S_Z (1<<WORLD_Z_M)
#define WORLD__RC_S_Y 3

#define WORLD_Z_SH (WORLD_X_M+4)
#define WORLD_Y_SH ((WORLD_X_M+4)*2)

const int WORLD__R_S_X = (16 * WORLD__RC_S_X);
const int WORLD__R_S_Z = (16 * WORLD__RC_S_Z);
const int WORLD__R_S_Y = (16 * WORLD__RC_S_Y);

const int WORLD__C_CH_X = (WORLD__RC_S_X / 2);
const int WORLD__C_CH_Z = (WORLD__RC_S_Z / 2);

namespace World {
	void init();
	void set_center_chunk(int x, int z, int y);
	void set_block(int x, int y, int z, unsigned short id);
	unsigned short get_block(int x, int y, int z);
	unsigned short get_block_l(int x, int y, int z);
	bool check_load(int cx, int cy, int cz);
	//void set_center_chunk_y();
	void ImGui_draw();
#ifndef MRE
	void test_draw(sf::Texture& temp_preview, sf::RenderWindow& window);
#endif // !MRE
	void load_chunck_section(int x, int y, int z, unsigned short *a);
}
