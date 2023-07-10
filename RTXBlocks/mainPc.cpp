#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>

#include <cmath>
#include <corecrt_math_defines.h>

#include <omp.h>

#include <iostream>
#include <fstream>

#include "Sock.h"
#include "Protocol.h"
#include "PacketMaker.h"
#include "PacketOpener.h"
#include "World.h"
#include "Render.h"
#include "BlockPalette.h"
#include "PlayerInfo.h"
#include "Player.h"
#include "Time.h"
#include "Chat.h"
#include "Keyboard.h"
#include "Entity.h"
#include "Main.h"

#define  VM_KEY_FN 106

extern int render_c;

extern unsigned short* main_canvas_buff;
extern int_fixed* main_deep_buff;

extern unsigned short* main_canvas_buff2;
extern int_fixed* main_deep_buff2;

void click_buttom(int key, int ev) {
	//void run_keyboard_callback(VMINT event, VMINT keycode);
	if (key != VM_KEY_FN)
		Main::handle_keyevt((ev == 0) ? (VM_KEY_EVENT_DOWN) : (VM_KEY_EVENT_UP), key);
	//cout << "Key: " << key << ", ev: " << ev << '\n';
}
struct Keys {
	char name[20] = "";
	int code = 0;
};
Keys keys[3 * 7] =
{
	{"Left S",VM_KEY_LEFT_SOFTKEY},
	{"UP",VM_KEY_UP},
	{"Right S",VM_KEY_RIGHT_SOFTKEY},
	{"LEFT",VM_KEY_LEFT},
	{"OK",VM_KEY_OK},
	{"RIGHT",VM_KEY_RIGHT},
	{"",VM_KEY_FN},
	{"Down",VM_KEY_DOWN},
	{" ",VM_KEY_FN},
	{"1.,",VM_KEY_NUM1},
	{"2abc",VM_KEY_NUM2},
	{"3def",VM_KEY_NUM3},
	{"4ghi",VM_KEY_NUM4},
	{"5jkl",VM_KEY_NUM5},
	{"6mno",VM_KEY_NUM6},
	{"7pqrs",VM_KEY_NUM7},
	{"8tuv",VM_KEY_NUM8},
	{"9wxyz",VM_KEY_NUM9},
	{"*",VM_KEY_STAR},
	{"0",VM_KEY_NUM0},
	{"#",VM_KEY_POUND},
};

void keyboard() {
	ImVec2 v = { 60,20 };
	ImGui::Begin("KeyBoard");
	for (int i = 0; i < 3 * 7; ++i) {
		if (i % 3 != 0)
			ImGui::SameLine();
		if (ImGui::Button(keys[i].name, v))
			click_buttom(keys[i].code, 1);
		if (ImGui::IsItemClicked())
			click_buttom(keys[i].code, 0);
	}
	ImGui::End();
}

void rkeyboard(sf::Event& event) {
	bool pr = 1;
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		pr = 0;
	case sf::Event::KeyReleased:
		switch (event.key.code)
		{
		case sf::Keyboard::Up:
			click_buttom(VM_KEY_UP, pr);
			break;
		case sf::Keyboard::Down:
			click_buttom(VM_KEY_DOWN, pr);
			break;
		case sf::Keyboard::Left:
			click_buttom(VM_KEY_LEFT, pr);
			break;
		case sf::Keyboard::Right:
			click_buttom(VM_KEY_RIGHT, pr);
			break;
		case sf::Keyboard::Numpad0:
			click_buttom(VM_KEY_NUM0, pr);
			break;
		case sf::Keyboard::Numpad1:
			click_buttom(VM_KEY_NUM7, pr);
			break;
		case sf::Keyboard::Numpad2:
			click_buttom(VM_KEY_NUM8, pr);
			break;
		case sf::Keyboard::Numpad3:
			click_buttom(VM_KEY_NUM9, pr);
			break;
		case sf::Keyboard::Numpad4:
			click_buttom(VM_KEY_NUM4, pr);
			break;
		case sf::Keyboard::Numpad5:
			click_buttom(VM_KEY_NUM5, pr);
			break;
		case sf::Keyboard::Numpad6:
			click_buttom(VM_KEY_NUM6, pr);
			break;
		case sf::Keyboard::Numpad7:
			click_buttom(VM_KEY_NUM1, pr);
			break;
		case sf::Keyboard::Numpad8:
			click_buttom(VM_KEY_NUM2, pr);
			break;
		case sf::Keyboard::Numpad9:
			click_buttom(VM_KEY_NUM3, pr);
			break;
		case sf::Keyboard::Home:
			click_buttom(VM_KEY_OK, pr);
			break;
		case sf::Keyboard::End:
			click_buttom(VM_KEY_LEFT_SOFTKEY, pr);
			break;
		case sf::Keyboard::PageUp:
			click_buttom(VM_KEY_RIGHT_SOFTKEY, pr);
			break;
		case sf::Keyboard::Slash:
			click_buttom(VM_KEY_STAR, pr);
			break;
		case sf::Keyboard::Multiply:
			click_buttom(VM_KEY_POUND, pr);
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}

sf::Clock cl;
int vm_get_tick_count() {
	return cl.getElapsedTime().asMilliseconds() & 0xFFFFFFFF;
}

void read_from_file_to_addr(const char* path, void** addr) {
	std::ifstream in(path, std::ios_base::binary);
	if (in.good()) {
		in.seekg(0, std::ios::end);
		int size = in.tellg();
		in.seekg(0, std::ios::beg);
		*addr = malloc(size);
		in.read((char*)*addr, size);
		in.close();
	}

}


int main() {
	const int v_w = 240, v_h = 320;

	main_canvas_buff = (unsigned short*)malloc(s_w * s_h * 2);
	main_deep_buff = (int_fixed*)malloc(s_w * s_h * 4);
	main_canvas_buff2 = (unsigned short*)malloc(s_w * s_h * 2);
	main_deep_buff2 = (int_fixed*)malloc(s_w * s_h * 4);

	Main::init_all();
	Main::init_all2();

	sf::RenderWindow window(sf::VideoMode(1280, 720), "RTXBlocks");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	{
		extern unsigned short* blocks;

		blocks = (unsigned short*)malloc(256 * 256 * 2 * 2);

		std::ifstream in("blocks_texture.bin", std::ios_base::binary);
		if (in.good()) {
			in.seekg(0, std::ios::end);
			int size = in.tellg();
			in.seekg(0, std::ios::beg);
			in.read((char*)blocks, size);
			in.close();
		}

		BlockPalette::biome_set();
	}

	auto full = sf::VideoMode::getFullscreenModes();

	std::cout << "Max of threads: " << omp_get_max_threads() << '\n';


	bool focus_f = false;

	sf::Clock deltaClock;
	while (window.isOpen()) {
		int ww = window.getSize().x, wh = window.getSize().y;
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			Keyboard::keyMaping(event);
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::GainedFocus:
				focus_f = true;
				break;
			case sf::Event::LostFocus:
				focus_f = false;
				break;
			case sf::Event::Resized:
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
				break;
				break;
			}
		}
		int d_time = deltaClock.getElapsedTime().asMilliseconds();
		if (d_time > 200)
			d_time = 200;

		ImGui::SFML::Update(window, deltaClock.restart());


		World::ImGui_draw();
		BlockPalette::draw_ImGui();
		PlayerInfo::imgui_draw();
		Time::draw_ImGui();
		Chat::draw_ImGui();
		Player::ImGui_draw();
		Entity::imgui_draw();

		keyboard();

		if (ImGui::Begin("Main")) {
			ImGui::Text("Delta time = %d", d_time);
			if (d_time)
				ImGui::Text("FPS = %d", 1000 / d_time);
		}
		ImGui::End();

		Main::game_loop(d_time);

		if (focus_f && !render_c)
			render_c = 1;
		if (render_c) {
			//Render::main_render();
			render_c--;
		}
		{
			sf::Texture tex;
			Render::render(tex);

			sf::Sprite sp(tex);
			sp.setScale(1, 1);
			window.draw(sp);

		}
		 {
			sf::Texture tex;
			Render::render2(tex);

			sf::Sprite sp(tex);
			sp.setScale(1, 1);
			sp.setPosition(s_w, 0);
			window.draw(sp);
		}
		//World::test_draw(temp_preview, window);

		ImGui::SFML::Render(window);
		window.display();
		window.clear();
		//after_main();
		Keyboard::update();
	}

	ImGui::SFML::Shutdown();

	Main::deinit_all();
}