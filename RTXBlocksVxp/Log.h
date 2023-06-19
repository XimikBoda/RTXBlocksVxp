#pragma once
#include <stdio.h>
extern "C" void LOGLOG(const char* file, const int line, const char* data);

namespace Log {
	void init();
}
#define LOG(x) {LOGLOG(__FILE__, __LINE__,#x);x;}
#define LOG_M(x) {LOGLOG(__FILE__, __LINE__,x);}