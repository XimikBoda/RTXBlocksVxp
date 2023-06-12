#include "Time.h"
#include <imgui.h>
#include <stdio.h>
#include <time.h>
static long long World_Age = 0;
static long long Time_of_day = 0;
namespace Time
{
	void draw_ImGui() {
		if (ImGui::Begin("Time")) {
			ImGui::Text("World Age: %lld", World_Age);
			ImGui::Text("Time of day: %lld (%lld)", Time_of_day, Time_of_day % 24000);
			ImGui::Text("");
			ImGui::Text("Day: %lld", World_Age / 24000);
			ImGui::Text(get_time_in_str());
		}
		ImGui::End();
	}

	char* get_time_in_str() {
		static char str[20];
		int time = Time_of_day % 24000;
		time += 6000;
		time %= 24000;
		time = time * 36 / 10;
		unsigned char h = time / 60 / 60;
		unsigned char m = (time / 60) % 60;
		unsigned char s = time % 60;

		sprintf(str, "%02d:%02d:%02d", h, m, s);
		return str;
	}
	
	void set_time(long long World_Age_, long long Time_of_day_) {
		World_Age = World_Age_;
		Time_of_day = Time_of_day_;
	}
	long long get_time() {
		return World_Age;
	}
	long long get_time_1970() {
		time_t ltime;
		time(&ltime);
		//printf("Current local time as unix timestamp: %li\n", ltime);

		//struct tm* timeinfo = gmtime(&ltime); /* Convert to UTC */
		//ltime = mktime(timeinfo); /* Store as unix timestamp */
		//printf("Current UTC time as unix timestamp: %li\n", ltime);

		return (long long)(ltime)*1000;
	}
};