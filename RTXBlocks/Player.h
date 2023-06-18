#pragma once

struct Player_s {
	enum Mode {
		Normal,
		Sprint,
		Sneaking
	};
	float x, y, z;
	float a, b; 
	float c;
	float acs=0;
	Mode mode = Normal;
	bool zoom;
	bool on_ground = true;
	bool fly;
	float get_h() {
		return (mode == Player_s::Sneaking ? 1.1f : 1.62f);
	}
	float get_speed() {
		switch (mode)
		{
		case Player_s::Normal:
			return 4.317f;
			break;
		case Player_s::Sprint:
			return 5.612f;
			break;
		case Player_s::Sneaking:
			return 1,295;
			break;
		default:
			return 1.f;
			break;
		}
	}
};
namespace Player
{
	void update(int d_time);
	void ImGui_draw();
	bool cheack_collision(unsigned short id);
};

