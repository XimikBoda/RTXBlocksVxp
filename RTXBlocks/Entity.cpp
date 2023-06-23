#include "Entity.h"
#include "PlayerInfo.h"
#ifndef MRE
#include <imgui.h>
#endif // !MRE

entity temp[100];
int entity_info_count = 0;

namespace Entity {
#ifndef MRE
	void imgui_draw() {
		if (ImGui::Begin("Entities")) {
			for (int i = 0; i < entity_info_count; ++i) {
				ImGui::Text("Id: %d", temp[i].id);
				ImGui::Text(temp[i].uuid.get_string());
				ImGui::Text("XYZ: %f, %f, %f", float(temp[i].x), float(temp[i].y), float(temp[i].z));
				ImGui::Text("YPY: %f, %f, %f", temp[i].yaw, temp[i].pitch, temp[i].hyaw);
				//if(temp[i].is_player)
				//	ImGui::Text(PlayerInfo::get_find_by_uuid(&temp[i].uuid)->nick);
				ImGui::Text("");
			}
		}
		ImGui::End();
	}
#endif // !MRE
	entity* get_null() {
		return &temp[entity_info_count];
	}
	void get_make_null_as_entity() {
		entity_info_count++;
	}
	entity* get_find_by_uuid(UUID* uuid) {
		for (int i = 0; i < entity_info_count; ++i)
			if (temp[i].uuid == *uuid)
				return &temp[i];
		return 0;
	}
	entity* get_find_by_id(int id) {
		for (int i = 0; i < entity_info_count; ++i)
			if (temp[i].id == id)
				return &temp[i];
		return 0;
	}
	void delete_by_id(int id) {
		for (int i = 0; i < entity_info_count; ++i)
			if (temp[i].id == id) {
				for (int j = i; j < entity_info_count - 1; ++j)
					temp[j] = temp[j + 1];
				entity_info_count--;
				break;
			}
	}
};