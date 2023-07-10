#include "Chat.h"
#include "Time.h"
#include "Protocol.h"
#include "Render.h"
#include <string>
#ifndef MRE
#include <imgui.h>
#include <imgui_internal.h>
static std::string chat_text;
#else
#include "vmgraph.h"
#endif // !MRE
extern GameState gameState;
const int chat_lines = 10;
const int ch_in_w = s_w / 6;

static char text[256] = {};
static char input_text_[256] = { '>' };
static char* input_text = input_text_ + 1;
static char chat_text_[chat_lines * ch_in_w] = {};
static int chat_text_times[chat_lines] = {};
static int input_text_len = 0, chat_text_pos = 0;
static int down_pos = s_h;

extern int input_cursor_x, input_cursor_y;

#ifndef MRE
int vm_get_tick_count();
void vm_graphic_fill_rect(unsigned char* buf, int x, int y, int w, int h, unsigned short c1, unsigned short c2);
#endif

namespace Chat
{
	bool need_scroll = false;
#ifndef MRE
	void draw_ImGui() {
		if (ImGui::Begin("Chat")) {
			ImGui::InputTextMultiline("###3", &chat_text[0], chat_text.size() + 1, ImVec2(0, 0), ImGuiInputTextFlags_ReadOnly | (1 << 17));
			if (need_scroll) {
				//ImGui::BeginChild(ImGui::GetID("History"));
				//////ImGui::SetScrollHere(0.999f);
				//ImGui::SetScrollY(ImGui::GetScrollMaxY());
				//ImGui::EndChild();
				//need_scroll = 0;
			}
			bool c = ImGui::InputText("##2", text, 256, ImGuiInputTextFlags_EnterReturnsTrue);
			if (c) {
				Protocol::send_message(text);
				text[0] = 0;
			}
			//ImGui::SameLine();
		}
		ImGui::End();
	}
#endif


	static int min(int a, int b) {
		return a < b ? a : b;
	}

	void draw_input(unsigned short* buf) {
		int len = input_text_len + 1;
		int lines = (len) / ch_in_w + 1;
		vm_graphic_fill_rect((unsigned char*)buf, 0, s_h - lines * 11, s_w, lines * 11, 0, 0);
		for (int i = 0; i < lines; ++i)
			Render::draw_text_white_by_len(buf, 0, s_h - lines * 11 + i * 11, input_text_ + i * ch_in_w, min(ch_in_w, len - (i * ch_in_w)));
		input_cursor_y = (s_h - 11) / 11;
		input_cursor_x = ((len) % ch_in_w);
		down_pos = s_h - (lines + 1) * 11;
	}
	void draw_chat(unsigned short* buf) {
		int time = vm_get_tick_count();
		if (gameState != ChatS)
			//	vm_graphic_fill_rect((VMUINT8*)buf, 0, down_pos - chat_lines * 11, s_w, chat_lines * 11, 0, 0);
			//else
			down_pos = s_h;
		for (int i = 0; i < chat_lines; ++i)
			if (gameState == ChatS || time - chat_text_times[i] < 5000)
				Render::draw_text_white_with_black_by_len(buf, 0, down_pos - (chat_lines - i) * 11, chat_text_ + i * ch_in_w, ch_in_w);
	}
	void chat_input(const char* str) {
		for (; *str; ++str) {
			char ch = *str;
			if (ch == 127)
				input_text_len--;
			else if (ch == '\n') {
				input_text[input_text_len] = 0;
				Protocol::send_message(input_text);
				input_text_len = 0;
			}
			else
				input_text[input_text_len++] = ch;
		}
	}
	static void new_line() {
		memmove(chat_text_, chat_text_ + ch_in_w, ch_in_w * (chat_lines - 1));
		memmove(chat_text_times, chat_text_times + 1, 4 * (chat_lines - 1));
		chat_text_pos = 0;
		memset(chat_text_ + ch_in_w * 9, 0, ch_in_w);
	}

	void add_to_chat(const char ch) {
#ifdef MRE
		if (chat_text_pos == ch_in_w && ch != '\n')
			new_line();
		if (ch == '\n') {
			chat_text_pos = ch_in_w;
			return;
		}
		chat_text_[ch_in_w * 9 + chat_text_pos++] = ch;
		chat_text_times[chat_lines - 1] = vm_get_tick_count();
#endif // MRE
	}
	void add_text(char* who, char* text) {
#ifndef MRE
		chat_text = chat_text + "\n[" + who + "] " + text;
		need_scroll = 1;
#endif // !MRE

		add_to_chat('[');
		for (; *who; ++who)
			add_to_chat(*who);
		add_to_chat(']');
		add_to_chat(' ');
		for (; *text; ++text)
			add_to_chat(*text);
		add_to_chat('\n');
	}
	void add_text(char* text) {
#ifndef MRE
		chat_text += text;
		chat_text += '\n';
		need_scroll = 1;
#endif // !MRE
		for (; *text; ++text)
			add_to_chat(*text);
		add_to_chat('\n');
	}
};
