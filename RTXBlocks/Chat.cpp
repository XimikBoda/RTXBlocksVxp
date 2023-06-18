#include "Chat.h"
#include "Time.h"
#include "Protocol.h"
#include <string>
#ifndef MRE
#include <imgui.h>
#include <imgui_internal.h>
static std::string chat_text;
#endif // !MRE
static char text[256] = {};

namespace Chat
{
	bool need_scroll = false;
#ifndef MRE
	void draw_ImGui() {
		if (ImGui::Begin("Chat")) {
			ImGui::InputTextMultiline("###3", &chat_text[0], chat_text.size() + 1, ImVec2(0, 0), ImGuiInputTextFlags_ReadOnly | (1<<17));
			if(need_scroll){
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
#endif // !MRE
	void add_text(char* who, char* text) {
#ifndef MRE
		chat_text = chat_text + "\n[" + who + "] " + text;
		need_scroll = 1;
#endif // !MRE
	}
	void add_text(char* text) {
#ifndef MRE
		chat_text += text;
		chat_text += '\n';
		need_scroll = 1;
#endif // !MRE
	}
};
