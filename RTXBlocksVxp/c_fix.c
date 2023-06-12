#include "vmsys.h"
#ifndef WIN32
void* malloc(int size){
	return vm_malloc(size);
}
void free(void*prt){
	return vm_free(prt);
}

void _sbrk(){}
void _write(){}
void _close(){}
void _lseek(){}
void _open(){}
void _read(){}
void _exit(){}
void _getpid(){}
void _kill(){}
void _fstat(){}
void _isatty(){}
#endif