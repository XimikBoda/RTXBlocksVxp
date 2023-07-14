#pragma once
#include "UIBase.h"
#include "UIButton.h"


class UIConfigMenu : UIBase
{
public:
	UIButton startButton = UIButton(5, 100, 50, 14, "Start");

	void Draw(unsigned short* buf) override;
	void KeyboardEvent(int event, int keycode) override;
};

