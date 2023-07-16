#pragma once
#include "UIBase.h"
#include "UIButton.h"
#include "UITextBox.h"

extern char Nickname[17];

class UIConfigMenu : UIBase
{
public:
	UITextBox nicknameInput = UITextBox(this, 5, 0, 200, 14, Nickname, 16);
	UIButton startButton = UIButton(this, 5, 0, 50, 14, "Start", (ActOfClick)&UIConfigMenu::ClickStart);
	UIBase* uiarray[4] = 
	{ 
		(UIConfigMenu::UIBase*)&(UIConfigMenu::nicknameInput), //0
		0, //1
		0, //2
		(UIConfigMenu::UIBase*)&(UIConfigMenu::startButton) //3
	};
	char uiarray_moves[4][4] = 
	{
		{3, 1, 0, 0},
		{0, 2, 1, 1},
		{1, 3, 2, 2},
		{2, 0, 3, 3}
	};
	int cur_element = 0;

	UIConfigMenu();

	void Draw(unsigned short* buf) override;
	void KeyboardEvent(int event, int keycode) override;

	void ClickStart();

private:
	void MoveFocus(int new_id);
};

