#pragma once
namespace Time
{
	void draw_ImGui();
	void set_time(long long World_Age_, long long Time_of_day_);
	char* get_time_in_str();
	long long get_time();
	long long get_time_1970();
};

