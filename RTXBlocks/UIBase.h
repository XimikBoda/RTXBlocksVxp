#pragma once

const unsigned short gray_color = 0x632C;

class UIBase
{
public:
	int x = 0, y = 0;
	int w = 20, h = 20;
	bool isFocused = false;
	UIBase* parent = 0;

	virtual void Draw(unsigned short* buf) {};
	virtual void QDraw(unsigned short* buf) {};
	virtual void KeyboardEvent(int event, int keycode) {};
	virtual void TextEvent(const char* str) {};
	virtual void OKClick() {};
};

