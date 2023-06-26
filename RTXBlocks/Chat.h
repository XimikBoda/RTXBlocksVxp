#pragma once
namespace Chat
{
	void draw_ImGui();
	void add_text(char* who, char* text);
	void add_text(char* text);
	void draw_input(unsigned short* buf);
	void draw_chat(unsigned short* buf);
	void chat_input(const char* str);
};

