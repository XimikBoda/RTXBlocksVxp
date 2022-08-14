#include "Keyboard.h"

static int key[118];

namespace Keyboard
{
	void init(){
		for(int i=0;i<118;++i)
		key[i]=1;
	}

	bool pres(int a){
		if(key[a+9]!=1)
			return true;
		else
			return false;
	}
	bool click(int a){
		if(key[a+9]!=1){
			key[a+9]=1;
			return true;
		}else
			return false;
	}
	bool at_least_one_key(){
		for(int i=0;i<118;++i)
			if(key[i]!=1)
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

};
