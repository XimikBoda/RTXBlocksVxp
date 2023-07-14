#include "UIConfigMenu.h"
#include "Render.h"

void UIConfigMenu::Draw(unsigned short* buf)
{
	Render::draw_fill_rect(buf, 0, 0, s_w, s_h, 0x0000);

	int y = 3;

	Render::draw_text_white_centered(buf, 0, y, s_w, "RTXBlocks");

	Render::draw_fill_rect(buf, 0, y+13, s_w, 1, 0xFFFF);

	y += 20;
	Render::draw_text_white(buf, 5, y, "Nickname:");
	y += 15;
	Render::draw_text_white(buf, 5, y, "---------");

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
	startButton.Draw(buf);

}

void UIConfigMenu::KeyboardEvent(int event, int keycode)
{

}

