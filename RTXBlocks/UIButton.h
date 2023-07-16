#pragma once
#include "UIBase.h"

typedef void (UIBase::*ActOfClick)();

class UIButton : public UIBase
{
public:
	char text[200] = "";

	ActOfClick actOfClick = 0;

	UIButton(UIBase* parent, int x, int y, int w, int h, const char* text, ActOfClick actOfClick);
	void QDraw(unsigned short* buf) override;
	void OKClick() override;
};

