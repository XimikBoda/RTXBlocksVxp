#pragma once
namespace BlockPalette
{
	void init();
	void draw_ImGui();
	void biome_set();
	unsigned char get_block_id(unsigned short id);
	unsigned short get_big_block_id(unsigned char id);
	void draw_collisions();
};

