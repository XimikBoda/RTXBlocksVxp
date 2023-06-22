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

//#include "Keyboard.h"
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

//const float pi = M_PI;

#define VM_COLOR_888_TO_565(r, g, b)	(((r & 0xf8) + ((g & 0xe0) >> 5)) << 8) + ((g & 0x1c) << 3) + (b >> 3)

unsigned short* steve;

typedef int int_fixed;

int render_c = 0;

void init_all() {
	BlockPalette::init();
	Keyboard::init();
	Sock::init();
	Protocol::init();
	PacketMaker::init();
	PacketOpener::init();
	World::init();
	Render::init();
}

void deinit_all() {
	Sock::deinit();
	//Protocol::deinit();
	PacketMaker::deinit();
	PacketOpener::deinit();
}

int main() {
	init_all();
	const int v_w = 240, v_h = 320;

	//{
	//	unsigned char* data, * data2;
	//	std::ifstream in("ura.bin", std::ios::in | std::ios::binary | std::ios::ate);
	//	if (!in.good())
	//		return 0;
	//	//ifstream::pos_type in;
	//	long long size = in.tellg(), p = 0;
	//	in.seekg(0, std::ios::beg);
	//	in.read(reinterpret_cast<char*>(&world3[0]), 32 * 128 * 128);
	//	in.close();
	//}

	sf::Image blocks_;
	blocks_.loadFromFile("terarian.png");
	sf::Color* blocks = (sf::Color*)blocks_.getPixelsPtr();

	/*{
		sf::Image my_skin_;
		my_skin_.loadFromFile("steve.png");
		my_skin_.
		sf::Color* my_skin = (sf::Color*)my_skin_.getPixelsPtr();
	}*/

	sf::Texture temp_preview;
	temp_preview.loadFromFile("texture_previev1.png");

	sf::RenderWindow window(sf::VideoMode(1280, 720), "RTXBlocks");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	{
		extern unsigned short* blocks;

		blocks = (unsigned short*)malloc(256*256*2*2);

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

	Protocol::connect();

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

		Protocol::update();
		Player::update(d_time);

		World::ImGui_draw();
		BlockPalette::draw_ImGui();
		PlayerInfo::imgui_draw();
		Time::draw_ImGui();
		Chat::draw_ImGui();
		Player::ImGui_draw();
		Entity::imgui_draw();

		if(ImGui::Begin("Main")){
			ImGui::Text("Delta time = %d", d_time);
			if(d_time)
				ImGui::Text("FPS = %d", 1000/d_time);
		}
		ImGui::End();

		if (focus_f && !render_c)
			render_c = 1;
		if (render_c) {
			Render::main_render();
			render_c--;
		}
		{
			sf::Texture tex;
			Render::render(tex);

			sf::Sprite sp(tex);
			sp.setScale(1, 1);
			window.draw(sp);

		}
		/*{
			sf::Texture tex;
			Render::render2(tex);

			sf::Sprite sp(tex);
			sp.setScale(1, 1);
			sp.setPosition(s_w, 0);
			window.draw(sp);
		}*/
		//World::test_draw(temp_preview, window);

		ImGui::SFML::Render(window);
		window.display();
		window.clear();
		//after_main();
		Keyboard::update();
	}

	ImGui::SFML::Shutdown();

	deinit_all();
}