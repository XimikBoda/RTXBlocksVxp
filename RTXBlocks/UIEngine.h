#pragma once
#include "UIBase.h"

const int max_uistack_size = 20;

class UIEngine
{
	UIBase* uistack[max_uistack_size] = {0};
	int uistack_size = 0;


public:
	unsigned short* def_buf;
	void init();
	void PushUI(UIBase* ui);
	void PopUI();
	void KeyboardEvent(int event, int keycode);
	void TextEvent(const char* str);
	void Draw(unsigned short* buf);
};

