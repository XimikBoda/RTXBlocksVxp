#include "PlayerInfo.h"
#ifndef MRE
#include <imgui.h>
#endif // !MRE

UUID my_uuid;

static player_info temp[100];
static int player_info_count = 0;

namespace PlayerInfo
{
#ifndef MRE
	void imgui_draw() {
		if (ImGui::Begin("PlayerInfo")) {
			for (int i = 0; i < player_info_count; ++i) {
				ImGui::Text(temp[i].uuid.get_string());
				ImGui::Text(temp[i].nick);
				ImGui::Text("Ping: %d", temp[i].ping);
				ImGui::Text("Gamemode: %d", temp[i].gamemode);
				ImGui::Text("");
			}
		}
		ImGui::End();
	}
#endif // !MRE
	player_info* get_null() {
		return &temp[player_info_count];
	}
	void get_make_null_as_pl() {
		player_info_count++;
	}
	player_info* get_find_by_uuid(UUID* uuid) {
		for (int i = 0; i < player_info_count; ++i)
			if (temp[i].uuid == *uuid)
				return &temp[i];
		return 0;
	}
	void delete_by_uuid(UUID* uuid) {
		for (int i = 0; i < player_info_count; ++i)
			if (temp[i].uuid == *uuid) {
				for (int j = i; j < player_info_count - 1; ++j)
					temp[j] = temp[j + 1];
				player_info_count--;
				break;
			}
	}
};

