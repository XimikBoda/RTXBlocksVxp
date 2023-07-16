#include "UIConfigMenu.h"
#include "Render.h"
#include "Keyboard.h"

UIConfigMenu::UIConfigMenu()
{
	if (uiarray[cur_element])
		uiarray[cur_element]->isFocused = 1;
}

void UIConfigMenu::Draw(unsigned short* buf)
{
	Render::draw_fill_rect(buf, 0, 0, s_w, s_h, 0x0000);

	int y = 3;

	Render::draw_text_white_centered(buf, 0, y, s_w, "RTXBlocks");

	Render::draw_fill_rect(buf, 0, y + 13, s_w, 1, 0xFFFF);

	y += 20;
	Render::draw_text_white(buf, 5, y, "Nickname:");
	y += 15;

	nicknameInput.y = y;
	nicknameInput.QDraw(buf);

	y += 20;
	Render::draw_text_white(buf, 5, y, "Relay server:");
	y += 15;
	Render::draw_text_white(buf, 5, y, "---------");

	y += 20;
	Render::draw_text_white(buf, 5, y, "Minecraft server:");
	y += 15;
	Render::draw_text_white(buf, 5, y, "---------");

	y += 20;
	//Render::draw_text_white(buf, 5, y, "(Play)");
	startButton.y = y;
	startButton.QDraw(buf);

}

void UIConfigMenu::KeyboardEvent(int event, int keycode)
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

		case VM_KEY_OK:
			if (uiarray[cur_element])
				uiarray[cur_element]->OKClick();
			break;
		}
		break;
	}
	startButton.OKClick();
}

void UIConfigMenu::ClickStart()
{

}

void UIConfigMenu::MoveFocus(int new_id)
{
	if (new_id > sizeof(uiarray) / sizeof(uiarray[0]) || cur_element == new_id)
		return;

	if (uiarray[cur_element])
		uiarray[cur_element]->isFocused = 0;

	cur_element = new_id;

	if (uiarray[cur_element])
		uiarray[cur_element]->isFocused = 1;
}

