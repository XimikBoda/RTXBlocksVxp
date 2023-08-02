#pragma once
#include "UIBase.h"
class UITextBox : public UIBase {
public:
	char* text;
	int text_max_len = 16;
	bool input_mode = false;
	int cursor_pos = 0;

	UITextBox(UIBase* parent, int x, int y, int w, int h, char* text, int text_max_len);

	void Draw(unsigned short* buf) override;
	void QDraw(unsigned short* buf) override;
	void KeyboardEvent(int event, int keycode) override;
	void TextEvent(const char* str) override;
	void OKClick() override;
};

