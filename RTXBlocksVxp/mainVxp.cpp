#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "ResID.h"
#include "vm4res.h"	
#include "vmres.h"
#include "vmtimer.h"
#include "string.h"

#include <cstdlib>

#include "../RTXBlocks/Main.h"
#include "../RTXBlocks/Keyboard.h"
#include "Log.h"

#ifdef GCC
#define SAVE_ENTER \
	asm("str SP, %0" : "=m" (return_sp));\
	return_address = __builtin_return_address(0);
#else
#define SAVE_ENTER
#endif

VMUINT16* layer_bufs[2] = { 0,0 };
VMINT layer_hdls[2] = { -1,-1 };

int scr_w = 0, scr_h = 0;

volatile void* return_address = 0;
volatile unsigned long* return_sp = 0;



extern unsigned short* main_canvas_buff;
extern int_fixed* main_deep_buff;

extern unsigned short* main_canvas_buff2;
extern int_fixed* main_deep_buff2;

void handle_sysevt(VMINT message, VMINT param);
void handle_keyevt(VMINT event, VMINT keycode);
void handle_penevt(VMINT event, VMINT x, VMINT y);
void show_error_and_exit(const char* text);

int p_time = 0;
void main_timer_(int tid) {
	int n_time = vm_get_tick_count();
	int d_time = n_time - p_time;
	p_time = n_time;

	Main::game_loop(d_time);
	vm_graphic_flush_layer(layer_hdls, 2);
	Keyboard::update();
}
void main_timer(int tid) {
	SAVE_ENTER
	main_timer_(tid);
}

#ifdef GCC
#define fix_malloc vm_malloc
#else
#define fix_malloc malloc
#endif // GCC


int read_from_file_to_addr(const char* path_, void** addr) {
	char path[200];
	sprintf(path, "e:\\RTXBlocks\\%s", path_);
	VMWCHAR wstr[200];
	vm_gb2312_to_ucs2(wstr, 400, (VMSTR)path);
	VMUINT red = 0, size = 0;

	VMFILE f = vm_file_open(wstr, MODE_READ, 1);
	if (f < 0) {
		//char tmp[100] = "";
		//sprintf("%s not found", path);
		//show_error_and_exit("Some file not found");
		*addr = 0;
		return 0;
	}
	vm_file_getfilesize(f, &size);
	*addr = fix_malloc(size);
	vm_file_read(f, *addr, size, &red);
	vm_file_close(f);
	return size;
}

void write_from_addr_to_file(const char* path_, void* addr, int size) {
	char path[200];
	sprintf(path, "e:\\RTXBlocks\\%s", path_);
	VMWCHAR wstr[200];
	vm_gb2312_to_ucs2(wstr, 400, (VMSTR)path);
	VMUINT red = 0;

	VMFILE f = vm_file_open(wstr, MODE_CREATE_ALWAYS_WRITE, 1);
	if (f < 0) {
		//char tmp[100] = "";
		//sprintf("%s not found", path);
		//show_error_and_exit("Some file not found");
		return;
	}
	vm_file_write(f, addr, size, &red);
	vm_file_close(f);
}

void vm_main_(void) {
	scr_w = vm_graphic_get_screen_width();
	scr_h = vm_graphic_get_screen_height();

	vm_switch_power_saving_mode(turn_off_mode);

	srand(vm_get_tick_count());

	Log::init();

	vm_reg_sysevt_callback(handle_sysevt);
	vm_reg_keyboard_callback(handle_keyevt);
	vm_reg_pen_callback(handle_penevt);

	main_deep_buff = (int_fixed*)vm_malloc(s_w * s_h * 4);
	main_deep_buff2 = (int_fixed*)vm_malloc(s_w * s_h * 4);

}
void vm_main(void) {
	SAVE_ENTER
	vm_main_();
}


void handle_sysevt_(VMINT message, VMINT param) {
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
			main_canvas_buff = layer_bufs[0];
			main_canvas_buff2 = layer_bufs[1];
			Main::init_all();
			Main::init_all2();
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
		if (layer_hdls[0] != -1) {
			vm_graphic_delete_layer(layer_hdls[1]);
			vm_graphic_delete_layer(layer_hdls[0]);
		}
		Main::deinit_all();
		break;
	}
}

void handle_sysevt(VMINT message, VMINT param) {
	SAVE_ENTER
	handle_sysevt_(message, param);
}


void handle_keyevt(VMINT event, VMINT keycode) {
	SAVE_ENTER
#ifdef WIN32   //Fix for MoDIS
	if (VM_KEY_NUM1 <= keycode && keycode <= VM_KEY_NUM3)
		keycode += 6;
	else if (VM_KEY_NUM7 <= keycode && keycode <= VM_KEY_NUM9)
		keycode -= 6;
#endif
	Main::handle_keyevt(event, keycode);
}

void handle_penevt(VMINT event, VMINT x, VMINT y) {
	SAVE_ENTER
	Main::handle_penevt(event, x, y);
}

void input_exit(VMINT state, VMWSTR text) {
	SAVE_ENTER
	vm_exit_app();
}

extern "C" void show_error_and_exit_c(const char* text) {
	show_error_and_exit(text);
}

void show_error_and_exit(const char* text) {
	VMWCHAR wstr[200];
	vm_gb2312_to_ucs2(wstr, 200, (VMSTR)text);
	vm_input_text2(wstr, 0, input_exit);
}
