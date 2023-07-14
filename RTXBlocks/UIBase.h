#pragma once

const unsigned short gray_color = 0x632C;

class UIBase
{
public:
	bool isFocused = false;
	virtual void Draw(unsigned short* buf) = 0;
	virtual void KeyboardEvent(int event, int keycode) {};
};

