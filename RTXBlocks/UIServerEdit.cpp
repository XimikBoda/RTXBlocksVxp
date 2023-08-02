#include "UIServerEdit.h"
#include "Render.h"
#include "Keyboard.h"
#include "UIEngine.h"
#ifdef MRE
#include "vmstdlib.h"
#include "string.h"
#endif // MRE


extern UIEngine uiengine;

const char* MinecraftServerEl::get_name() {
	if (name.length())
		return name.c_str();
	return host.c_str();
}

void UIServerEdit::start(MinecraftServerEl* el)
{
	this->el = el;
	memcpy(name, el->name.c_str(), 51);
	memcpy(host, el->host.c_str(), 51);
	sprintf(port, "%hu", el->port);
	uiengine.PushUI((UIBase*)this);
}

void UIServerEdit::Draw(unsigned short* buf)
{
	Render::draw_fill_rect(buf, 0, 0, s_w, s_h, 0x0000);

	int y = 3;
	int x = 5;

	Render::draw_text_white_centered(buf, 0, y, s_w, "RTXBlocks");

	Render::draw_fill_rect(buf, 0, y + 13, s_w, 1, 0xFFFF);

	y += 20;
	Render::draw_text_white(buf, 5, y, "Server name:");
	y += 15;

	nameImput.y = y;

	y += 20;
	Render::draw_text_white(buf, 5, y, "Server addres:");
	y += 15;

	hostImput.y = y;

	y += 20;
	Render::draw_text_white(buf, 5, y, "Server port:");
	y += 15;

	portImput.y = y;

	y += 20;
	saveButton.y = y;

	for (int i = 0; i < sizeof(uiarray) / sizeof(uiarray[0]); ++i)
		uiarray[i]->QDraw(buf);
}

void UIServerEdit::KeyboardEvent(int event, int keycode)
{
	switch (event) {
	case VM_KEY_EVENT_UP:
		switch (keycode) {
		case VM_KEY_UP:
			MoveFocus(uiarray_moves[cur_element][0]);
			break;
		case VM_KEY_DOWN:
			MoveFocus(uiarray_moves[cur_element][1]);
			break;
		case VM_KEY_LEFT:
			MoveFocus(uiarray_moves[cur_element][2]);
			break;
		case VM_KEY_RIGHT:
			MoveFocus(uiarray_moves[cur_element][3]);
			break;
		case VM_KEY_RIGHT_SOFTKEY:
			uiengine.PopUI();
			break;

		case VM_KEY_OK:
			if (uiarray[cur_element])
				uiarray[cur_element]->OKClick();
			break;
		}
		break;
	}
}

void UIServerEdit::MoveFocus(int new_id)
{
	if (new_id > sizeof(uiarray) / sizeof(uiarray[0]) || cur_element == new_id)
		return;

	if (uiarray[cur_element])
		uiarray[cur_element]->isFocused = 0;

	cur_element = new_id;

	if (uiarray[cur_element])
		uiarray[cur_element]->isFocused = 1;
}

void UIServerEdit::ClickSave()
{
	unsigned short iport = 0;
	if (sscanf(port, "%hu", &iport) == 1) {
		el->name = name;
		el->host = host;
		el->port = iport;
		uiengine.PopUI();
	}
}
