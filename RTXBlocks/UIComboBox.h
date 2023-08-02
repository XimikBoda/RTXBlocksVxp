#pragma once
#include "UIBase.h"

typedef int (UIBase::* GetListLen)();
typedef const char* (UIBase::* GetListElement)(int id);

class UIComboBox : public UIBase
{
public:
	GetListLen getListLen = 0;
	GetListElement getListElement = 0;

	int cur_id = 0;

	UIComboBox(UIBase* parent, int x, int y, int w, int h, int cur_id, GetListLen getListLen, GetListElement getListElement);

	void Draw(unsigned short* buf) override;
	void QDraw(unsigned short* buf) override;
	void KeyboardEvent(int event, int keycode) override;
	void OKClick() override;
};

