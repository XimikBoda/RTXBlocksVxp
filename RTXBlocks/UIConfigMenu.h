#pragma once
#include "UIBase.h"
#include "UIButton.h"
#include "UITextBox.h"
#include "UIComboBox.h"
#include <vector>

extern char Nickname[17];

class UIConfigMenu : UIBase
{
public:
	UITextBox nicknameInput = UITextBox(this, 5, 0, 200, 14, Nickname, 16);

	UIComboBox relayServerSelecter = UIComboBox(this, 5, 0, 200, 14);
	UIButton rss_add = UIButton(this, 5, 0, 14, 14, "+", 0);
	UIButton rss_remove = UIButton(this, 5, 0, 14, 14, "-", 0);
	UIButton rss_up = UIButton(this, 5, 0, 14, 14, "\30", 0);
	UIButton rss_edit = UIButton(this, 5, 0, 14, 14, "R", 0);
	UIButton rss_down = UIButton(this, 5, 0, 14, 14, "\31", 0);

	UIComboBox serverSelecter = UIComboBox(this, 5, 0, 200, 14);
	UIButton ss_add = UIButton(this, 5, 0, 14, 14, "+", 0);
	UIButton ss_remove = UIButton(this, 5, 0, 14, 14, "-", 0);
	UIButton ss_up = UIButton(this, 5, 0, 14, 14, "\30", 0);
	UIButton ss_edit = UIButton(this, 5, 0, 14, 14, "R", 0);
	UIButton ss_down = UIButton(this, 5, 0, 14, 14, "\31", 0);

	UIButton startButton = UIButton(this, 5, 0, 50, 14, "Start", (ActOfClick)&UIConfigMenu::ClickStart);
	UIBase* uiarray[14] = 
	{ 
		(UIConfigMenu::UIBase*)&(UIConfigMenu::nicknameInput), //0
		(UIConfigMenu::UIBase*)&(UIConfigMenu::relayServerSelecter), //1
		(UIConfigMenu::UIBase*)&(UIConfigMenu::rss_add), //2
		(UIConfigMenu::UIBase*)&(UIConfigMenu::rss_remove), //3
		(UIConfigMenu::UIBase*)&(UIConfigMenu::rss_up), //4
		(UIConfigMenu::UIBase*)&(UIConfigMenu::rss_edit), //5
		(UIConfigMenu::UIBase*)&(UIConfigMenu::rss_down), //6
		(UIConfigMenu::UIBase*)&(UIConfigMenu::serverSelecter), //7
		(UIConfigMenu::UIBase*)&(UIConfigMenu::ss_add), //8
		(UIConfigMenu::UIBase*)&(UIConfigMenu::ss_remove), //9
		(UIConfigMenu::UIBase*)&(UIConfigMenu::ss_up), //10
		(UIConfigMenu::UIBase*)&(UIConfigMenu::ss_edit), //11
		(UIConfigMenu::UIBase*)&(UIConfigMenu::ss_down), //12
		(UIConfigMenu::UIBase*)&(UIConfigMenu::startButton) //13
	};
	char uiarray_moves[14][4] = 
	{
		{13, 1, 0, 0},//0
		{0, 2, 1, 1},//1
		{1, 7, 6, 3},//2
		{1, 7, 2, 4},//3
		{1, 7, 3, 5},//4
		{1, 7, 4, 6},//5
		{1, 7, 5, 2},//6
		{2, 8, 7, 7},//7
		{7, 13, 12, 9},//8
		{7, 13, 8, 10},//9
		{7, 13, 9, 11},//10
		{7, 13, 10, 12},//11
		{7, 13, 11, 8},//12
		{8, 0, 13, 13},//13
	};

	int cur_element = 0;

	UIConfigMenu();

	void Draw(unsigned short* buf) override;
	void KeyboardEvent(int event, int keycode) override;


private:
	void MoveFocus(int new_id);

	void ClickStart();
};

