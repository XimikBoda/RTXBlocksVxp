#include "UIComboBox.h"
#include "Render.h"
#include "Keyboard.h"
#include "string.h"
#include "UIEngine.h"

extern UIEngine uiengine;

static void draw_triangle(unsigned short* buf, int x1, int y1, int x2, int x3, int y3, unsigned short color) {
	// 1---2
	//  \ /
	//   3
	int miy = y1 < y3 ? y1 : y3;
	int may = y1 > y3 ? y1 : y3;
	for (int y = miy; y <= may; ++y) {
		if (y < 0 || y >= s_h)
			continue;
		int mix = (y - y1) * (x3 - x1) / (y3 - y1) + x1;
		int max_ = (y - y1) * (x3 - x2) / (y3 - y1) + x2;
		for (int x = mix; x <= max_; ++x) {
			if (x < 0 || x >= s_w)
				continue;
			buf[x + y * s_w] = color;
		}
	}
}

UIComboBox::UIComboBox(UIBase* parent, int x, int y, int w, int h, int cur_id, GetListLen getListLen, GetListElement getListElement)
{
	this->parent = parent;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->cur_id = cur_id;
	this->getListLen = getListLen;
	this->getListElement = getListElement;
}

void UIComboBox::Draw(unsigned short* buf)
{
	unsigned char color = isFocused ? gray_color : 0x0000;
	Render::draw_fill_rect_with_borders(buf, x, y, w, h, color, 0xFFFF);

	Render::draw_fill_rect_with_borders(buf, x + w - 15, y, 15, h, color, 0xFFFF);

	{
		int x1 = x + w - 15 + 4;
		int x2 = x + w + -5;
		int x3 = (x1 + x2) / 2;
		int y1 = y + h / 2 + (x3 - x1), y3 = y + h / 2 - (x3 - x1);
		draw_triangle(buf, x1, y1, x2, x3, y3, 0xFFFF);
	}

	if (parent && getListLen && getListElement) {
		int size = (parent->*getListLen)();
		if (!size)
			Render::draw_text_white(buf, x + 3, y + (h - 11) / 2 + 1, "(empty)");
		else {
			if (cur_id < 0)
				cur_id = 0;
			if (cur_id >= size)
				cur_id = size - 1;

			const char* text = (parent->*getListElement)(cur_id);
			Render::draw_text_white(buf, x + 3, y + (h - 11) / 2 + 1, text);
		}


		Render::draw_fill_rect_with_borders(buf, x, y + h, w, 13 * size, 0x0000, 0xFFFF);

		Render::draw_fill_rect(buf, x + 1, y + h + cur_id * 13 + 1, w - 2, 13 - 2, color);

		for (int i = 0, y = this->y + h + 2; i < size; ++i, y += 13)
			Render::draw_text_white(buf, x + 3, y,
				(parent->*getListElement)(i));

	}
}

void UIComboBox::QDraw(unsigned short* buf)
{
	unsigned char color = isFocused ? gray_color : 0x0000;
	Render::draw_fill_rect_with_borders(buf, x, y, w, h, color, 0xFFFF);

	Render::draw_fill_rect_with_borders(buf, x + w - 15, y, 15, h, color, 0xFFFF);

	{
		int x1 = x + w - 15 + 4;
		int x2 = x + w + -5;
		int x3 = (x1 + x2) / 2;
		int y1 = y + h / 2 - (x3 - x1), y3 = y + h / 2 + x3 - x1;
		draw_triangle(buf, x1, y1, x2, x3, y3, 0xFFFF);
	}

	if (parent && getListLen && getListElement) {
		int size = (parent->*getListLen)();
		if (!size)
			Render::draw_text_white(buf, x + 3, y + (h - 11) / 2 + 1, "(empty)");
		else {
			if (cur_id < 0)
				cur_id = 0;
			if (cur_id >= size)
				cur_id = size - 1;

			const char* text = (parent->*getListElement)(cur_id);
			Render::draw_text_white(buf, x + 3, y + (h - 11) / 2 + 1, text);
		}
	}
}

void UIComboBox::KeyboardEvent(int event, int keycode)
{
	switch (event)
	{
	case VM_KEY_EVENT_UP:
		switch (keycode)
		{
		case VM_KEY_UP:
			if (cur_id > 0)
				cur_id--;
			break;
		case VM_KEY_DOWN:
			if (parent && getListLen && cur_id <= (parent->*getListLen)())
				cur_id++;
			break;
		case VM_KEY_OK:
		case VM_KEY_RIGHT_SOFTKEY:
			uiengine.PopUI();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void UIComboBox::OKClick()
{
	uiengine.PushUI((UIBase*)this);
}
