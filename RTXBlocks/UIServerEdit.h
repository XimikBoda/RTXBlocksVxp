#pragma once
#include "UIBase.h"
#include "UIButton.h"
#include "UITextBox.h"
#include <string>

struct MinecraftServerEl
{
	std::string name;
	std::string host;
	unsigned short port = 25565;
	const char* get_name();
};

class UIServerEdit : public UIBase
{
	char name[51] = "";
	char host[51] = "";
	char port[11] = "";
	UITextBox nameImput = UITextBox(this, 5, 0, 200, 14, name, 50);
	UITextBox hostImput = UITextBox(this, 5, 0, 200, 14, host, 50);
	UITextBox portImput = UITextBox(this, 5, 0, 200, 14, port, 10);

	UIButton saveButton = UIButton(this, 5, 0, 50, 14, "Save", (ActOfClick)&UIServerEdit::ClickSave);

	UIBase* uiarray[4] =
	{
		(UIServerEdit::UIBase*)&(UIServerEdit::nameImput), //0
		(UIServerEdit::UIBase*)&(UIServerEdit::hostImput), //1
		(UIServerEdit::UIBase*)&(UIServerEdit::portImput), //2
		(UIServerEdit::UIBase*)&(UIServerEdit::saveButton), //3
	};
	char uiarray_moves[14][4] =
	{
		{3, 1, 0, 0},//0
		{0, 2, 1, 1},//1
		{1, 3, 2, 2},//2
		{2, 0, 3, 3},//3
	};
	int cur_element = 0;

	MinecraftServerEl* el = 0;
public:
	void start(MinecraftServerEl* el);
	void Draw(unsigned short* buf) override;
	void KeyboardEvent(int event, int keycode) override;

private:
	void MoveFocus(int new_id);

	void ClickSave();
};

