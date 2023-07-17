#pragma once
#include "UIBase.h"
class UIComboBox : public UIBase
{
public:
	UIComboBox(UIBase* parent, int x, int y, int w, int h);

	void Draw(unsigned short* buf) override;
	void QDraw(unsigned short* buf) override;
	void KeyboardEvent(int event, int keycode) override;
	void OKClick() override;
};

