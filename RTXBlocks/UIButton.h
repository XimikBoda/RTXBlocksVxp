#pragma once
#include "UIBase.h"

class UIButton : UIBase
{
public:
	int x = 0, y = 0;
	int w = 20, h = 20;
	char text[200] = "";

	UIButton(int x, int y, int w, int h, const char* text);
	void Draw(unsigned short* buf) override;
};

