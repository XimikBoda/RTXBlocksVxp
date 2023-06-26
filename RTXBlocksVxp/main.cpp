#include "main.h"
#include "../RTXBlocks/Sock.h"
#include "../RTXBlocks/Protocol.h"
#include "../RTXBlocks/PacketMaker.h"
#include "../RTXBlocks/PacketOpener.h"
#include "../RTXBlocks/World.h"
#include "../RTXBlocks/Render.h"
#include "../RTXBlocks/BlockPalette.h"
#include "../RTXBlocks/PlayerInfo.h"
#include "../RTXBlocks/Player.h"
#include "../RTXBlocks/Time.h"
#include "../RTXBlocks/Chat.h"
#include "../RTXBlocks/Keyboard.h"
#include "Log.h"
//#include "../RTXBlocks/Profont6x11.h"

int scr_w = 0, scr_h =0; //screen width and height
VMUINT16 *layer_bufs[2] = {0,0};
VMINT layer_hdls[2] = {-1,-1};

extern unsigned int tcp_in_statistic, tcp_out_statistic;
extern unsigned int udp_in_statistic, udp_out_statistic;

extern int_fixed* main_deep_buff;
extern int_fixed* main_deep_buff2;

GameState gameState = GameState::PlayS;

unsigned short* steve;

int render_c = 0;

void handle_sysevt(VMINT message, VMINT param);
void handle_keyevt(VMINT event, VMINT keycode);

void init_all(){
	Log::init();
	BlockPalette::init();
	Keyboard::init();
	Sock::init();
	Protocol::init();
	PacketMaker::init();
	PacketOpener::init();
	World::init();
}

void deinit_all(){
	Sock::deinit();
	PacketMaker::deinit();
	PacketOpener::deinit();
}

int p_time = 0;
void main_timer(int tid){
	int n_time = vm_get_tick_count();
	int d_time = n_time - p_time;
	p_time = n_time;
	if (d_time > 200)
		d_time = 200;

	Render::clear_to_transparent_color(layer_bufs[1]);

	Protocol::update();
	Player::update(d_time);
	Render::main_render();
	memcpy(main_deep_buff2, main_deep_buff, s_w*s_h*4);
	Render::second_render(layer_bufs[1], main_deep_buff2);
	Keyboard::update();

	{
		char tmp[300];
		//sprintf(tmp, "Fps: %.1f, dt: %d", (d_time? 1000.f / float(d_time): NAN), d_time);
		sprintf(tmp, "Fps: %d, dt: %d", (d_time ? 1000 / d_time : 0), d_time);
		Render::draw_text_white(layer_bufs[1], 0, 0 , tmp);

		sprintf(tmp, "TCP i/o: %dKB/%dKB, UDP i/o: %dKB/%dKB", tcp_in_statistic / 1024, tcp_out_statistic / 1024, 
			udp_in_statistic / 1024, udp_out_statistic / 1024);
		Render::draw_text_white(layer_bufs[1], 0, 11, tmp);
	}
	extern unsigned short* blocks;

	vm_graphic_flush_layer(layer_hdls, 2);
}

void read_from_file_to_addr(const char* path, void ** addr) {
	VMWCHAR wstr[200];
	vm_gb2312_to_ucs2(wstr, 200, (VMSTR)path);
	VMUINT red = 0, size = 0;

	VMFILE f = vm_file_open(wstr, MODE_READ, 1);
	if (f < 0) {
		char tmp[100]="";
		sprintf("%s not found", path);
		show_error_and_exit("Some file not found");
		return;
	}
	vm_file_getfilesize(f, &size);
	*addr = vm_malloc(size);
	vm_file_read(f, *addr, size, &red);
	vm_file_close(f);

}

void vm_main(void){
	scr_w = vm_graphic_get_screen_width(); 
	scr_h = vm_graphic_get_screen_height();

	vm_switch_power_saving_mode(turn_off_mode);

	srand(vm_get_tick_count());

	init_all();

	Protocol::connect();
	
	vm_reg_sysevt_callback(handle_sysevt);
	vm_reg_keyboard_callback(handle_keyevt);

	extern unsigned short* blocks;
	read_from_file_to_addr("e:\\RTXBlocks\\blocks_texture.bin", (void**)&blocks);
	read_from_file_to_addr("e:\\RTXBlocks\\steve.bin", (void**)&steve);

	BlockPalette::biome_set();
}


void handle_sysevt(VMINT message, VMINT param) {
	switch (message) {
	case VM_MSG_CREATE:
	case VM_MSG_ACTIVE:
		layer_hdls[0] = vm_graphic_create_layer(0, 0, scr_w, scr_h, -1);
		layer_hdls[1] = vm_graphic_create_layer(0, 0, scr_w, scr_h, tr_color);
		
		vm_graphic_set_clip(0, 0, scr_w, scr_h);

		layer_bufs[0] = (VMUINT16*)vm_graphic_get_layer_buffer(layer_hdls[0]);
		layer_bufs[1] = (VMUINT16*)vm_graphic_get_layer_buffer(layer_hdls[1]);

		vm_kbd_set_mode(VM_KEYPAD_2KEY_NUMBER);

		vm_switch_power_saving_mode(turn_off_mode);
		if (message == VM_MSG_CREATE) {
			Render::init();
			vm_create_timer_ex(10, main_timer);
		}
		break;
		
	case VM_MSG_PAINT:
		break;
		
	case VM_MSG_INACTIVE:
		vm_switch_power_saving_mode(turn_on_mode);
		//if( layer_hdls[0] != -1 ){
		//	vm_graphic_delete_layer(layer_hdls[1]);
		//	vm_graphic_delete_layer(layer_hdls[0]);
		//}
		break;	
	case VM_MSG_QUIT:
		if( layer_hdls[0] != -1 ){
			vm_graphic_delete_layer(layer_hdls[1]);
			vm_graphic_delete_layer(layer_hdls[0]);
		}
		deinit_all();
		break;	
	}
}


void handle_keyevt(VMINT event, VMINT keycode) {
#ifdef WIN32   //Fix for MoDIS
	if(VM_KEY_NUM1<=keycode&&keycode<=VM_KEY_NUM3)
		keycode+=6;
	else if(VM_KEY_NUM7<=keycode&&keycode<=VM_KEY_NUM9)
		keycode-=6;
#endif
	if(gameState==PlayS)
		Keyboard::keyboard_event(keycode,event);
}

void input_exit(VMINT state, VMWSTR text){
	vm_exit_app();
}

void show_error_and_exit(const char *text){
	VMWCHAR wstr[200];
	vm_gb2312_to_ucs2(wstr,200,(VMSTR)text);
	vm_input_text2(wstr,0,input_exit);
}
