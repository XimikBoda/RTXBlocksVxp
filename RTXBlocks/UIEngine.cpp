#include "UIEngine.h"

void UIEngine::init()
{
	uistack_size = 0;
}

void UIEngine::PushUI(UIBase* ui) {
	if (uistack_size + 1 == max_uistack_size)
		return;
	uistack[uistack_size++] = ui;
	Draw(def_buf);
}

void UIEngine::PopUI() {
	if (uistack_size != 0)
		uistack_size--;
	Draw(def_buf);
}

void UIEngine::KeyboardEvent(int event, int keycode) {
	if (uistack_size && uistack[uistack_size - 1]) {
		uistack[uistack_size - 1]->KeyboardEvent(event, keycode);
		Draw(def_buf);
	}
}

void UIEngine::TextEvent(const char* str){
	if (uistack_size && uistack[uistack_size - 1]) {
		uistack[uistack_size - 1]->TextEvent(str);
		Draw(def_buf);
	}
}

void UIEngine::Draw(unsigned short* buf) {
	if (uistack_size && uistack[uistack_size-1])
		uistack[uistack_size-1]->Draw(buf);
}