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

int scr_w = 0, scr_h =0; //screen width and height
VMUINT8 *layer_bufs[2] = {0,0};
VMINT layer_hdls[2] = {-1,-1};

int render_c = 0;

void handle_sysevt(VMINT message, VMINT param);
void handle_keyevt(VMINT event, VMINT keycode);

void init_all(){
	Keyboard::init();
	Sock::init();
	Protocol::init();
	PacketMaker::init();
	PacketOpener::init();
	World::init();
}

void deinit_all(){
	Sock::deinit();
	//Protocol::deinit();
	PacketMaker::deinit();
	PacketOpener::deinit();
}

void temp_timer(int tid){
	Protocol::update();
}

void vm_main(void){
	scr_w = vm_graphic_get_screen_width(); 
	scr_h = vm_graphic_get_screen_height();

	srand(vm_get_tick_count());

	init_all();

	Protocol::connect();
	
	vm_reg_sysevt_callback(handle_sysevt);
	vm_reg_keyboard_callback(handle_keyevt);

	vm_create_timer_ex(10, temp_timer);
}




void handle_sysevt(VMINT message, VMINT param) {
	switch (message) {
	case VM_MSG_CREATE:
	case VM_MSG_ACTIVE:
		layer_hdls[0] = vm_graphic_create_layer(0, 0, scr_w, scr_h, -1);
		layer_hdls[1] = vm_graphic_create_layer(0, 0, scr_w, scr_h, tr_color);
		
		vm_graphic_set_clip(0, 0, scr_w, scr_h);

		layer_bufs[0]=vm_graphic_get_layer_buffer(layer_hdls[0]);
		layer_bufs[1]=vm_graphic_get_layer_buffer(layer_hdls[1]);

		vm_switch_power_saving_mode(turn_off_mode);
		break;
		
	case VM_MSG_PAINT:
		break;
		
	case VM_MSG_INACTIVE:
		vm_switch_power_saving_mode(turn_on_mode);
		if( layer_hdls[0] != -1 ){
			vm_graphic_delete_layer(layer_hdls[1]);
			vm_graphic_delete_layer(layer_hdls[0]);
		}
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
