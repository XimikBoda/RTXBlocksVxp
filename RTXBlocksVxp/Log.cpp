#include "Log.h"
#include "vmsys.h"
#include "vmio.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include <stdio.h>
#include <string.h>


VMFILE f;
char fSTR[500];
VMUINT p;

extern "C" void LOGLOG(const char* file, const int line, const char* data) {
#ifdef WIN32
	sprintf_s(fSTR, 500, "%s:%d:%s\n", file, line, data);
#else
	sprintf(fSTR, "%s:%d:%s\n", file, line, data);
#endif
	vm_file_write(f, fSTR, strlen(fSTR), &p);
	vm_file_commit(f);
}
extern "C" void log_init_from_c() {
	Log::init();
}

namespace Log {
	void init() {
		VMWCHAR s[50];
		vm_gb2312_to_ucs2(s, 50, "e:\\RTXBlocks.log");
		f = vm_file_open(s, MODE_CREATE_ALWAYS_WRITE, 0);
	}
}