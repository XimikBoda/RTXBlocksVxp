#include "UIComboBox.h"
#include "Render.h"
#include "Keyboard.h"
#include "string.h"
#include "UIEngine.h"

extern UIEngine uiengine;

UIComboBox::UIComboBox(UIBase* parent, int x, int y, int w, int h)
{
	this->parent = parent;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void UIComboBox::Draw(unsigned short* buf)
{
}

void UIComboBox::QDraw(unsigned short* buf)
{
	unsigned char color = isFocused ? gray_color : 0x0000;
	Render::draw_fill_rect_with_borders(buf, x, y, w, h, color, 0xFFFF);

	Render::draw_fill_rect_with_borders(buf, x + w - 15, y, 15, h, color, 0xFFFF);
}

void UIComboBox::KeyboardEvent(int event, int keycode)
{
}

void UIComboBox::OKClick()
{
	uiengine.PushUI((UIBase*)this);
}
