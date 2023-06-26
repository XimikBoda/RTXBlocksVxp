#include "Keyboard.h"

static char key[118];

namespace Keyboard
{
	void init() {
		for (int i = 0; i < 118; ++i)
			key[i] = 1;
	}

	bool pres(int a) {
		if (key[a + 9] != 1)
			return true;
		else
			return false;
	}
	bool click(int a) {
		if (key[a + 9] != 1) {
			key[a + 9] = 1;
			return true;
		}
		else
			return false;
	}
	bool at_least_one_key() {
		for (int i = 0; i < 118; ++i)
			if (key[i] != 1)
				return true;
		return false;
	}
	void update() {
		for (int i = 0; i < 118; ++i)
			if (key[i] == 5)
				key[i] = 1;
	}
	void keyboard_event(int k, int ev) {
		if (key[k + 9] != 1 && ev == 1)
			key[k + 9] = 5;
		else
			key[k + 9] = ev;
	}
#ifndef MRE
	void keyMaping(sf::Event ev) {
		int p = 0;
		if (ev.type == sf::Event::EventType::KeyPressed)
			p = 2;
		else if (ev.type == sf::Event::EventType::KeyReleased)
			p = 1;
		else
			return;
		//printf("%d\n", p);
		switch (ev.key.code)
		{
		case sf::Keyboard::W:
			keyboard_event(VM_KEY_NUM2, p);
			break;
		case sf::Keyboard::A:
			keyboard_event(VM_KEY_NUM4, p);
			break;
		case sf::Keyboard::S:
			keyboard_event(VM_KEY_NUM8, p);
			break;
		case sf::Keyboard::D:
			keyboard_event(VM_KEY_NUM6, p);
			break;
		case sf::Keyboard::Up:
			keyboard_event(VM_KEY_UP, p);
			break;
		case sf::Keyboard::Down:
			keyboard_event(VM_KEY_DOWN, p);
			break;
		case sf::Keyboard::Left:
			keyboard_event(VM_KEY_LEFT, p);
			break;
		case sf::Keyboard::Right:
			keyboard_event(VM_KEY_RIGHT, p);
			break;
		case sf::Keyboard::Space:
			keyboard_event(VM_KEY_OK, p);
			break;
		case sf::Keyboard::Q:
			keyboard_event(VM_KEY_NUM1, p);
			break;
		case sf::Keyboard::Z:
			keyboard_event(VM_KEY_NUM3, p);
			break;
		case sf::Keyboard::LShift:
			keyboard_event(VM_KEY_NUM7, p);
			break;
		default:
			break;
		}
	}
#endif // MRE
};
