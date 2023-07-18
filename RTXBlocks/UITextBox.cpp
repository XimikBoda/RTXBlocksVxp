#include "UITextBox.h"
#include "Render.h"
#include "Keyboard.h"
#include "string.h"
#include "UIEngine.h"
#include "T2Input.h"

extern UIEngine uiengine;
extern T2Input t2input;

extern int render_c;

extern int input_cursor_x, input_cursor_y;

UITextBox::UITextBox(UIBase* parent, int x, int y, int w, int h, char* text, int text_max_len)
{
	this->parent = parent;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->text = text;
	this->text_max_len = text_max_len;
}

void UITextBox::Draw(unsigned short* buf)
{
	unsigned char color = isFocused ? gray_color : 0x0000;
	Render::draw_fill_rect_with_borders(buf, x, y, w, h, color, 0xFFFF);

	int len = strlen(text);
	int ty = y + (h - 11) / 2 + 1;

	Render::draw_text_white_by_len(buf, x + 3, ty, text, cursor_pos);

	Render::draw_fill_rect(buf, x + 3 + 6 * cursor_pos, ty - 1, 1, 13, 0xFFFF);

	input_cursor_x = x + 3 + 6 * cursor_pos + 1;
	input_cursor_y = ty;

	Render::draw_text_white_by_len(buf, x + 3 + 6 * cursor_pos, ty, text + cursor_pos, len - cursor_pos);
}

void UITextBox::QDraw(unsigned short* buf)
{
	unsigned char color = isFocused ? gray_color : 0x0000;
	Render::draw_fill_rect_with_borders(buf, x, y, w, h, color, 0xFFFF);

	Render::draw_text_white(buf, x + 3, y + (h - 11) / 2 + 1, text);

}

void UITextBox::KeyboardEvent(int event, int keycode)
{
	if (keycode == VM_KEY_LEFT || keycode == VM_KEY_RIGHT || keycode == VM_KEY_OK)
		switch (event)
		{
		case VM_KEY_EVENT_UP:
			switch (keycode)
			{
			case VM_KEY_LEFT:
				if (cursor_pos > 0)
					cursor_pos--;
				break;
			case VM_KEY_RIGHT:
				if (cursor_pos < strlen(text))
					cursor_pos++;
				break;
			case VM_KEY_OK:
				uiengine.PopUI();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	t2input.handle_keyevt(event, keycode);
}

void UITextBox::TextEvent(const char* str)
{
	for (int i = 0; str[i]; ++i) {
		if (str[i] == 127) {
			if (cursor_pos > 0) {
				memmove(text + cursor_pos - 1, text + cursor_pos, strlen(text) + 1 - cursor_pos);
				--cursor_pos;
			}
		}
		else if (strlen(text) <= text_max_len) {
			memmove(text + cursor_pos + 1, text + cursor_pos, strlen(text) + 1 - cursor_pos);
			text[cursor_pos] = str[i];
			++cursor_pos;
		}
	}
}

void UITextBox::OKClick()
{
	cursor_pos = strlen(text);
	uiengine.PushUI((UIBase*)this);
}
