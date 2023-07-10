#include "Player.h"
#include "World.h"
#include "main.h"
#include "Keyboard.h"
#include "Protocol.h"
#include "collisions.h"
#include <cmath>
#ifndef MRE
#include <imgui.h>
#endif // !MRE

Player_s player;

bool change_rotation = false;
bool change_position = false;

extern int start_chunk_x;
extern int start_chunk_z;
extern int start_chunk_y;

extern bool play;

extern int render_c;

bool sup_pos = 0;

float player_h = 1.8;

const float jump_acs = 5.4f;

static float drob(float a) {
	if (a >= 0)
		return a - int(a);
	else
		return (1.f + a - int(a));
}

namespace Player
{
#ifndef MRE
	void ImGui_draw() {
		if (ImGui::Begin("Player")) {
			if (ImGui::SliderFloat3("Pos", &player.x, 0, 128))
				change_position = true;

			if (ImGui::SliderFloat("A", &player.a, -4 * pi, 4 * pi))
				change_rotation = true;

			if (ImGui::SliderFloat("B", &player.b, 0, pi))
				change_rotation = true;

			ImGui::SliderFloat("C", &player.c, 0, pi * 2);

			ImGui::InputFloat("Asc", &player.acs);
			ImGui::Checkbox("On ground", &player.on_ground);
			ImGui::Checkbox("Zoom", &player.zoom);
			if (ImGui::Button("Respawn"))
				Protocol::respawn();
			//ImGui::SliderFloat("F", &camera.f, 0, 100);
			//ImGui::Checkbox("Auto camera", &pl_c);
		}
		ImGui::End();
	}
#endif // !MRE
	bool cheack_collision(unsigned short id) {
		return (collisions_data[id / 8] >> (id % 8)) & 1;
	}
	bool cheack_collision(int x, int y, int z) {
		return cheack_collision(World::get_block_l(x, y, z));
	}
	bool check_ground(float x, float y, float z) {
		float d_x = drob(x);
		float d_z = drob(z);
		float rd_x = 1.f - d_x;
		float rd_z = 1.f - d_z;

		/*for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j)
				printf("%d ", cheack_collision(World::get_block_l(x - 1 + i, y, z - 1 + j)));
			printf("\n");
		}
		printf("\n");*/
		if (cheack_collision(World::get_block_l(x, y, z)))
			return true;

		if (d_x < 0.300) {
			if (cheack_collision(World::get_block_l(x - 1, y, z)))
				return true;

			if (d_z < 0.300) {
				if (cheack_collision(World::get_block_l(x, y, z - 1)))
					return true;

				if (d_x * d_x + d_z * d_z < 0.3f * 0.3f)
					if (cheack_collision(World::get_block_l(x - 1, y, z - 1)))
						return true;
			}
			else if (d_z > 0.700) {
				if (cheack_collision(World::get_block_l(x, y, z + 1)))
					return true;

				if (d_x * d_x + rd_z * rd_z < 0.3f * 0.3f)
					if (cheack_collision(World::get_block_l(x - 1, y, z + 1)))
						return true;
			}
		}
		else if (d_x > 0.700) {
			if (cheack_collision(World::get_block_l(x + 1, y, z)))
				return true;

			if (d_z < 0.300) {
				if (cheack_collision(World::get_block_l(x, y, z - 1)))
					return true;

				if (rd_x * rd_x + d_z * d_z < 0.3f * 0.3f)
					if (cheack_collision(World::get_block_l(x + 1, y, z - 1)))
						return true;
			}
			else if (d_z > 0.700) {
				if (cheack_collision(World::get_block_l(x, y, z + 1)))
					return true;

				if (rd_x * rd_x + rd_z * rd_z < 0.3f * 0.3f)
					if (cheack_collision(World::get_block_l(x + 1, y, z + 1)))
						return true;
			}
		}
		return false;
	}
	void update(int d_time) {
		if (!play)
			return;
		if (Keyboard::pres(VM_KEY_UP) ||
			Keyboard::pres(VM_KEY_DOWN) ||
			Keyboard::pres(VM_KEY_LEFT) ||
			Keyboard::pres(VM_KEY_RIGHT))
			change_rotation = true;

		//if (Keyboard::click(VM_KEY_NUM3))
		if (int(player.x) % 16 == 0)
			sup_pos = World::check_load(int(player.x) / 16, int(player.y) / 16, int(player.z) / 16) &&
			World::check_load(int(player.x - 1) / 16, int(player.y) / 16, int(player.z) / 16);
		else
			sup_pos = World::check_load(int(player.x) / 16, int(player.y) / 16, int(player.z) / 16);

		if (player.acs && sup_pos)
			change_position = true;

		if (Keyboard::click(VM_KEY_NUM1))
			player.zoom = !player.zoom;

		//if (keyclick(VM_KEY_NUM7))
		//	player.sit = !player.sit;

		float rot_sp = pi / (!player.zoom ? 2 : 4);
		float walk_sp = player.get_speed();

		if (Keyboard::pres(VM_KEY_UP))
			player.b -= rot_sp * d_time / 1000.f;
		if (Keyboard::pres(VM_KEY_DOWN))
			player.b += rot_sp * d_time / 1000.f;
		if (player.b > pi)
			player.b = pi;
		if (player.b < 0.01)
			player.b = 0.01;

		if (player.zoom)
			player.c = 0.60;
		else
			player.c = 1.5;

		if (Keyboard::pres(VM_KEY_LEFT))
			player.a -= rot_sp * d_time / 1000.f;
		if (Keyboard::pres(VM_KEY_RIGHT))
			player.a += rot_sp * d_time / 1000.f;

		if (sup_pos) {
			if (player.acs < 0) {
				float player_new_y = player.y + player.acs * d_time / 1000.f;

				if (check_ground(player.x, player_new_y, player.z)) {
					player.y = 1 + (int)player_new_y;
					//player.acs = 0;
					player.on_ground = true;
				}
				else
					player.y = player_new_y;
			}
			if (drob(player.y) > 0.95 && check_ground(player.x, player.y, player.z))
				player.y = int(player.y) + 1;

			if (check_ground(player.x, player.y - 0.95f, player.z) && drob(player.y) < 0.1) {
				player.y = int(player.y);
				player.on_ground = true;
			}
			else
				player.on_ground = false;

			if (player.on_ground) {
				player.acs = 0;
				if (Keyboard::click(VM_KEY_OK) || Keyboard::click(VM_KEY_NUM5))
					player.acs = jump_acs;
			}
			else
				player.acs -= 9.8 * d_time / 1000.f;
		}
		else
			player.acs = 0;



		int forward = 0, rigth = 0;

		if (Keyboard::pres(VM_KEY_NUM2))
			forward += 1;
		if (Keyboard::pres(VM_KEY_NUM8))
			forward -= 1;
		if (Keyboard::pres(VM_KEY_NUM4))
			rigth -= 1;
		if (Keyboard::pres(VM_KEY_NUM6))
			rigth += 1;

		if (forward | rigth) {
			change_position = true;
			float walk_a = player.a;

			float walk_sp = player.get_speed();

			if (forward == 1)
				if (rigth == -1)
					walk_a -= pi / 4.f;
				else if (rigth == 1)
					walk_a += pi / 4.f;

			if (forward == 0)
				if (rigth == -1)
					walk_a -= pi / 2.f;
				else if (rigth == 1)
					walk_a += pi / 2.f;

			if (forward == -1)
				if (rigth == -1)
					walk_a -= pi - pi / 4.f;
				else if (rigth == 0)
					walk_a += pi;
				else if (rigth == 1)
					walk_a += pi - pi / 4.f;

			float xx = cos(walk_a) * walk_sp * d_time / 1000.f;
			float zz = sin(walk_a) * walk_sp * d_time / 1000.f;

			bool c1 = !check_ground(player.x + xx * 2, player.y + 0.1f, player.z + zz * 2);
			bool c2 = !check_ground(player.x + xx * 2, player.y + 1.1f, player.z + zz * 2);

			//printf("%d %d\n", c1, c2);

			if (c2) {
				if (c1)
					player.x += xx, player.z += zz;
				else if (forward == 1 && player.on_ground) {
					bool c3 = !check_ground(player.x + xx * 6, player.y + 0.1f, player.z + zz * 6);
					bool c4 = !check_ground(player.x + xx * 6, player.y + 1.1f, player.z + zz * 6);
					if (c4 && !c3)
						player.acs = jump_acs,
						player.on_ground = false;
				}
			}

		}
		if (sup_pos) {
			if (player.acs > 0) {
				float player_new_y = player.y + player.acs * d_time / 1000.f;

				if (check_ground(player.x, player_new_y + 1, player.z)) {
					player.y = (float)((int)player_new_y) + 2.f - player_h;
					//player.acs = 0;
					player.on_ground = false;
				}
				else
					player.y = player_new_y;
			}
		}

		if (change_rotation) {
			Protocol::Set_Player_Rotation();
			change_rotation = false;
			render_c = 1;
		}
		if (change_position) {
			Protocol::Set_Player_Position();
			change_position = false;
			render_c = 1;
		}
	}
};
