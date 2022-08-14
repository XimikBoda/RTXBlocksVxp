#pragma once

namespace Keyboard
{
	void init();

	bool pres(int a);
	bool click(int a);

	bool at_least_one_key();

	void update();

	void keyboard_event(int k, int ev);

};

