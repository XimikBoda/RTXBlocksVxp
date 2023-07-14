#pragma once
#include "UIBase.h"


class UIConfigMenu : UIBase
{
public:
	void Draw(unsigned short* buf);
	void KeyboardEvent(int event, int keycode);
};

