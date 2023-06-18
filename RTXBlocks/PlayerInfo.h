#pragma once
#include "UUID.h"

struct player_info {
	UUID uuid;
	char nick[17];
	int ping;
	unsigned char gamemode;
};

namespace PlayerInfo
{
	void imgui_draw();

	player_info* get_null();
	void get_make_null_as_pl();
	player_info* get_find_by_uuid(UUID* uuid);
	void delete_by_uuid(UUID* uuid);
};

