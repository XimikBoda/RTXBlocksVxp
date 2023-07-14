#pragma once
class UIBase
{
public:
	virtual void Draw(unsigned short* buf);
	virtual void KeyboardEvent(int event, int keycode);
};

