#include "Main.h"
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
#include "T2Input.h"
#include "UIEngine.h"
#include "UIConfigMenu.h"
#include <exception>
#include <vector>
#ifdef GCC
//#include <cxxabi.h>
#endif // MRE
#ifdef MRE
#include "vmstdlib.h"
#include "string.h"
#include "../RTXBlocksVxp/Log.h"
#endif // MRE

T2Input t2input;
UIEngine uiengine;
UIConfigMenu *uiconfigMenu;


int render_c = 0;
unsigned short* steve;
extern unsigned short* blocks;

unsigned short* main_canvas_buff;
int_fixed* main_deep_buff;

unsigned short* main_canvas_buff2;
int_fixed* main_deep_buff2;

extern unsigned int tcp_in_statistic, tcp_out_statistic;
extern unsigned int udp_in_statistic, udp_out_statistic;

GameState gameState = GameState::UI;

extern Player_s player;

extern int start_chunk_x;
extern int start_chunk_z;
extern int start_chunk_y;

#ifdef MRE
extern int layer_hdls[2];
#else
int layer_hdls[2] = { -1,-1 };
#endif // MRE

int read_from_file_to_addr(const char* path, void** addr);

namespace Main {
	void init_all() {
		BlockPalette::init();
		Keyboard::init();
		Sock::init();
		Protocol::init();
		PacketMaker::init();
		PacketOpener::init();
		World::init();

		t2input.init();
		uiengine.init();

		//Protocol::connect();
		
		read_from_file_to_addr("blocks_texture.bin", (void**)&blocks);
		read_from_file_to_addr("steve.bin", (void**)&steve);

		BlockPalette::biome_set();
	}

	void init_all2() {
		uiengine.def_buf = main_canvas_buff;
		Render::init();
		t2input.scr_buf = (unsigned char*)main_canvas_buff2;
		t2input.layer_handle = layer_hdls[1];

		uiconfigMenu = new UIConfigMenu;
		uiengine.PushUI((UIBase*)uiconfigMenu);
	}

	void deinit_all() {
		delete uiconfigMenu;
		Sock::deinit();
		PacketMaker::deinit();
		PacketOpener::deinit();
	}

	void game_loop(int d_time) {
		if (d_time > 200)
			d_time = 200;
		Render::clear_to_transparent_color(main_canvas_buff2);
		if (gameState == PlayS || gameState == ChatS) {

			Protocol::update();
			Player::update(d_time);
			Render::main_render();

			memcpy(main_deep_buff2, main_deep_buff, s_w * s_h * 4);
			Render::second_render(main_canvas_buff2, main_deep_buff2);
			{
				char tmp[300];
				sprintf(tmp, "Fps: %d, dt: %d", (d_time ? 1000 / d_time : 0), d_time);
				Render::draw_text_white(main_canvas_buff2, 0, 0, tmp);

				sprintf(tmp, "TCP i/o: %dKB/%dKB, UDP i/o: %dKB/%dKB", tcp_in_statistic / 1024, tcp_out_statistic / 1024,
					udp_in_statistic / 1024, udp_out_statistic / 1024);
				Render::draw_text_white(main_canvas_buff2, 0, 11, tmp);

				//sprintf(tmp, "x: %f, y: %f, z:%f", player.x + float(start_chunk_x * 16), 
				//	player.y + float(start_chunk_y * 16), player.z + float(start_chunk_z * 16));
				//Render::draw_text_white(layer_bufs[1], 0, 22, tmp);
			}
			if (gameState == ChatS)
				Chat::draw_input(main_canvas_buff2);
			Chat::draw_chat(main_canvas_buff2);
		}
		else if (gameState == UI) {
			uiengine.Draw(main_canvas_buff);
		}
		t2input.draw();
		Keyboard::update();
	}

		

	void handle_keyevt(int event, int keycode) {
		if (gameState == UI) {
			uiengine.KeyboardEvent(event, keycode);
		}else if (gameState == PlayS) {
			Keyboard::keyboard_event(keycode, event);
			if (keycode == VM_KEY_STAR && event == VM_KEY_EVENT_UP)
				gameState = ChatS;
		}
		else if (gameState == ChatS) {
			t2input.handle_keyevt(event, keycode);
			if (keycode == VM_KEY_STAR && event == VM_KEY_EVENT_UP)
				gameState = PlayS;
		}
	}

	void handle_penevt(int event, int x, int y) {
		t2input.handle_penevt(event, x, y);
	}

	void text_event(const char* str) {
		if (gameState == UI)
			uiengine.TextEvent(str);
		else if (gameState == ChatS)
			Chat::chat_input(str);
	}
}