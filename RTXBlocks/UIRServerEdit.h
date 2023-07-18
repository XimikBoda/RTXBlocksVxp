#pragma once
#include "UIBase.h"
#include "UIButton.h"
#include "UITextBox.h"
#include <string>

struct MinecraftRServerEl
{
	std::string host;
	unsigned short Tport = 25501;
	unsigned short Uport = 25501;
	const char* get_name();
};

class UIRServerEdit : public UIBase
{
	char host[51] = "";
	char tport[11] = "";
	char uport[11] = "";
	UITextBox hostImput = UITextBox(this, 5, 0, 200, 14, host, 50);
	UITextBox tportImput = UITextBox(this, 5, 0, 200, 14, tport, 10);
	UITextBox uportImput = UITextBox(this, 5, 0, 200, 14, uport, 10);

	UIButton saveButton = UIButton(this, 5, 0, 50, 14, "Save", (ActOfClick)&UIRServerEdit::ClickSave);

	UIBase* uiarray[4] =
	{
		(UIRServerEdit::UIBase*)&(UIRServerEdit::hostImput), //0
		(UIRServerEdit::UIBase*)&(UIRServerEdit::tportImput), //1
		(UIRServerEdit::UIBase*)&(UIRServerEdit::uportImput), //2
		(UIRServerEdit::UIBase*)&(UIRServerEdit::saveButton), //3
	};
	char uiarray_moves[14][4] =
	{
		{3, 1, 0, 0},//0
		{0, 2, 1, 1},//1
		{1, 3, 2, 2},//2
		{2, 0, 3, 3},//3
	};
	int cur_element = 0;

	MinecraftRServerEl* el = 0;
public:
	void start(MinecraftRServerEl* el);
	void Draw(unsigned short* buf) override;
	void KeyboardEvent(int event, int keycode) override;

private:
	void MoveFocus(int new_id);

	void ClickSave();
};

