#pragma once
#include "Main.h"

#define VM_PEN_EVENT_TAP				1
#define VM_PEN_EVENT_RELEASE			2
#define VM_PEN_EVENT_MOVE				3
#define VM_PEN_EVENT_LONG_TAP			4
#define VM_PEN_EVENT_DOUBLE_CLICK		5
#define VM_PEN_EVENT_REPEAT			6
#define VM_PEN_EVENT_ABORT          7

class T2Input
{
public:

	enum Input_mode {
		SMALL,
		FIRST_BIG,
		BIG,
		NUM,
	};

	enum State {
		MAIN,
		SECOND_CLICK,
		F_NUM,
		SET_MENU,
		CTRL,
		CTRL_SECOND_CLICK
	};

	Input_mode cur_input_mode;
	int last_input_time;
	int last_imput_key;

	State state;

	unsigned char* scr_buf;
	int layer_handle;

	int key_w, key_h;
	int keyboard_h;

	bool draw_kb;

	int current_key;

	short squares[8][2];

	int get_keycode(int x, int y);

	void send_c(const char* str);

	void numpad_input(int keycode);

	void show_current_pressed_key();

	void handle_penevt(int event, int x, int y);
	void handle_keyevt(int event, int keycode);

	void draw_xy_char(int x, int y, const char* str);
	void draw_xy_str(int x, int y, const char* str);
	void draw_xy_str_color(int x, int y, unsigned short textcolor, unsigned short backcolor, const char* str);

	void draw();
	void init();
	~T2Input(void);
};

