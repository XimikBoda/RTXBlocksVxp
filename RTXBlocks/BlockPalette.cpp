#include "BlockPalette.h"
#include "main.h"
#include "Player.h"
#ifndef MRE
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#else
#include "string.h"
#include"vmsys.h"
#define malloc vm_malloc
#define free vm_free
#endif // !MRE

unsigned short* blocks;

#define VM_COLOR_GET_RED(color16)		((color16 & 0xF800) >> 8)

#define VM_COLOR_GET_GREEN(color16)	    ((color16 & 0x07E0) >> 3)

#define VM_COLOR_GET_BLUE(color16)		((color16 & 0x001F) << 3)

#define VM_COLOR_888_TO_565(r, g, b)	(((r & 0xf8) + ((g & 0xe0) >> 5)) << 8) + ((g & 0x1c) << 3) + (b >> 3)

unsigned short multiply_color(unsigned short c, unsigned char r, unsigned char g, unsigned char b) {
	if(c== VM_COLOR_888_TO_565(255, 0, 255))
		return c;
	unsigned short rr = (VM_COLOR_GET_RED(c) * r) >> 8;
	unsigned short gg = (VM_COLOR_GET_GREEN(c) * g) >> 8;
	unsigned short bb = (VM_COLOR_GET_BLUE(c) * b) >> 8;
	return VM_COLOR_888_TO_565(rr, gg, bb);
}
#ifndef MRE
sf::Color mask = (sf::Color)(0xFF000000 | VM_COLOR_565_TO_888(VM_COLOR_888_TO_565(255, 0, 255)));
#endif // !MRE

unsigned short* small_palletre;

//static const unsigned short use_flag = 0x4000;

bool* use;

int min_n_use = 1;

unsigned short* big_palletre;


namespace BlockPalette {
	void init() {
		small_palletre = (unsigned short*)malloc(256 * sizeof(unsigned short));
		use = (bool*)malloc(256 * sizeof(bool));
		big_palletre = (unsigned short*)malloc(512 * sizeof(unsigned short));

		use[0] = true;

		for (int i = 0; i < 256; ++i) 
			small_palletre[i] = i;
		
		for (int i = 1; i < 256; ++i)
			use[i] = false;

		for (int i = 0; i < 512; ++i)
			big_palletre[i] = i;
	}
	void deinit() {
		free(small_palletre);
		free(use);
		free(big_palletre);
	}

	void draw_collisions() {
		for (int i = 0; i < 512; ++i) {
			unsigned short *p = &blocks[16 * 16 * i];
			unsigned short red = VM_COLOR_888_TO_565(255, 0, 0);
			unsigned short green = VM_COLOR_888_TO_565(0, 255, 0);

			unsigned short c = Player::cheack_collision(i)?red:green;

			for (int j = 0; j < 16; ++j) 
			{
				p[j] = c;
				p[16*15+j] = c;
				p[16*j] = c;
				p[16*j+15] = c;
			}
		}
	}
#ifndef MRE
	void draw_ImGui() {
		if (ImGui::Begin("BlockPalette")) {
			static sf::Texture global_tex;
			sf::Image im;
			sf::Uint8* pixels = new sf::Uint8[8 * 256 * 256 * 2];
			long* p = (long*)pixels;
			for (int i = 0; i < 16 * 2; ++i)
				for (int j = 0; j < 16; ++j)
					for (int q = 0; q < 16; ++q)
						for (int k = 0; k < 16; ++k) {
							unsigned short c = blocks[(j * 16 + k) + (i * 16 + q) * 256];
							if (c == VM_COLOR_888_TO_565(255, 0, 255))
								p[k + q * 16 + (i * 16 + 15 - j) * 16 * 16] = 0xFF000000;
							else
								p[k + q * 16 + (i * 16 + 15 - j) * 16 * 16] = 0xFF000000 | VM_COLOR_565_TO_888(c);
						}

			im.create(256, 256 * 2, pixels);
			//im.createMaskFromColor(mask);
			global_tex.loadFromImage(im);
			sf::Sprite sp(global_tex);
			sp.setScale(2, 2);
			ImGui::Image(sp);
			delete[] pixels;
		}
		ImGui::End();

		auto white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		auto green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		auto red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

		if (ImGui::Begin("BlockPalette small")) {
			ImGui::Text("Uses = %d", min_n_use);
			const char* numbers[16] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15" };
			if (ImGui::BeginTable("Small", 17)) {
				ImGui::TableSetupColumn("N");
				for (int i = 0; i < 16; ++i)
					ImGui::TableSetupColumn(numbers[i]);
				ImGui::TableHeadersRow();
				for (int i = 0; i < 16; ++i) {
					ImGui::TableNextColumn();
					ImGui::Text(numbers[i]);

					for (int j = 0; j < 16; ++j) {
						ImGui::TableNextColumn();
						ImGui::TextColored((use[i * 16 + j] ? green : white), "%d", small_palletre[i * 16 + j]);
					}
				}
			}
			ImGui::EndTable();
		}
		ImGui::End();

		if (ImGui::Begin("BlockPalette big")) {
			const char* numbers[32] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
			"16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31" };
			if (ImGui::BeginTable("Small", 17)) {
				ImGui::TableSetupColumn("N");
				for (int i = 0; i < 16; ++i)
					ImGui::TableSetupColumn(numbers[i]);
				ImGui::TableHeadersRow();
				for (int i = 0; i < 16 * 2; ++i) {
					ImGui::TableNextColumn();
					ImGui::Text(numbers[i]);

					for (int j = 0; j < 16; ++j) {
						ImGui::TableNextColumn();

						ImGui::TextColored((big_palletre[i * 16 + j] >=256?red:(use[big_palletre[i * 16 + j]] ? green : white)), "%d", big_palletre[i * 16 + j] & 0x1FF);
					}
				}
			}
			ImGui::EndTable();
		}
		ImGui::End();
	}
#endif // !MRE

	void swap_textures(unsigned short a, unsigned short b) {
		unsigned short tmp[16 * 16];
		memcpy(tmp, &blocks[a * 16 * 16], 16 * 16 * 2);
		memcpy(&blocks[a * 16 * 16], &blocks[b * 16 * 16], 16 * 16 * 2);
		memcpy(&blocks[b * 16 * 16], tmp, 16 * 16 * 2);
	}
	void swap(unsigned short &a, unsigned short &b) {
		unsigned short tmp=a;
		a = b;
		b = tmp;
	}


	unsigned char get_block_id(unsigned short id) {
		unsigned short tmp = big_palletre[id];
		if (tmp<256 && use[tmp])
			return tmp;
		else
		{
			if (min_n_use == 256)
				return 0;

			unsigned short new_id_s = min_n_use++;

			unsigned short new_id = small_palletre[new_id_s];

			//swap(big_palletre[id], big_palletre[new_id]);
			//swap_textures(big_palletre[id], big_palletre[new_id]);

			small_palletre[new_id_s] = id;
			if(tmp<256)
				small_palletre[tmp] = new_id;

			//big_palletre[id] |= use_flag;

			use[new_id_s] = true;

			return big_palletre[id];
		}
	}

	unsigned short get_big_block_id(unsigned char id) {
		return small_palletre[id];
	}

	void multiply_block(unsigned int id, unsigned char r, unsigned char g, unsigned char b) {
		for (int i = 0; i < 256; ++i)
			blocks[id*256+i] = multiply_color(blocks[id * 256 + i], r, g, b);
	}

	void biome_set() {
		//draw_collisions();
		int water_id = 16 + 6;
		int tree1_id = 12 * 16 + 3;
		int tree2_id = 12 * 16 + 4;
		int grass_ids[11] =
		{
			8,
			5 * 16 + 8, 5 * 16 + 9,
			12 * 16 + 2, 12 * 16 + 5, 12 * 16 + 9,
			13 * 16 + 9,
			15 * 16 + 10, 15 * 16 + 11
		};
		int foliage_ids[8] = 
		{
			3 * 16 + 2, 3 * 16 + 3, 3 * 16 + 4, 3 * 16 + 5, 3 * 16 + 6, 3 * 16 + 7, 3 * 16 + 8,
			12 * 16 + 6
		};
		multiply_block(water_id, 63, 118, 228);

		multiply_block(tree1_id, 56, 89, 56);
		multiply_block(tree2_id, 102, 133, 68);

		for (int i = 0; i < 11; ++i)
			multiply_block(grass_ids[i], 146, 190, 86);

		for (int i = 0; i < 8; ++i)
			multiply_block(foliage_ids[i], 115, 175, 42);
	}
}