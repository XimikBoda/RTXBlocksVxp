#include "UIButton.h"
#include "Render.h"
#include "string.h"

UIButton::UIButton(int x, int y, int w, int h, const char* text)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	strcpy(this->text, text);
}

void UIButton::Draw(unsigned short* buf)
{
	unsigned char color = isFocused ? gray_color : 0x0000;
	Render::draw_fill_rect_with_borders(buf, x, y, w, h, color, 0xFFFF);

	Render::draw_text_white_centered(buf, x, y + (h-11)/2 + 1, w, text);
}