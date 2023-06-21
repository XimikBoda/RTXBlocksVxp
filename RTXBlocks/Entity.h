#pragma once
#include"UUID.h"

struct entity {
	int id=0;
	UUID uuid;
	int type = 0;
	double x = 0, y = 0, z = 0;
	float pitch = 0, yaw = 0, hyaw = 0;
	int data = 0;
	short vx=0, vy=0, vz=0;
	bool is_player = 0;
	bool is_ground = 0;
};

namespace Entity
{
	void imgui_draw();
	entity* get_null();
	void get_make_null_as_entity();
	entity* get_find_by_uuid(UUID* uuid);
	entity* get_find_by_id(int id);
	void delete_by_id(int id);
};

