#include "UIConfigMenu.h"
#include "Render.h"

void UIConfigMenu::Draw(unsigned short* buf)
{
	Render::draw_fill_rect(buf, 0, 0, s_w, s_h, 0x0000);

	int y = 3;

	Render::draw_text_white_centered(buf, 0, y, s_w, "RTXBlocks");
}

void UIConfigMenu::KeyboardEvent(int event, int keycode)
{

}

