
#ifndef WIN32
#include "vmstdlib.h"
#include "vmsys.h"
//#include <stdio.h>
void LOGLOG(const char* file, const int line, const char* data);

#define LOG(x) {LOGLOG(__FILE__, __LINE__,#x);x;}
#define LOG_M(x) {LOGLOG(__FILE__, __LINE__,x);}

extern volatile void* return_address;
extern volatile unsigned long* return_sp;

void _sbrk(){}
void _write(int fd, const void* buffer, unsigned int count){
	LOG_M((const char*)buffer);
	return count;
}
void _close(){}
void _lseek(){}
void _open(){}
void _read(){}
void _exit(int status) {
	LOG_M("Exit");
	vm_exit_app();
	asm("ldr SP, %0" : "=m" (return_sp));
	asm("ldr PC, %0" : "=m" (return_address));
}
void _getpid(){}
void _kill(){}
void _fstat(){}
void _isatty(){}

int fiprintf(FILE* fd, const char* format, ...) {
	char buffer[200];
	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vm_vsprintf(buffer, format, aptr);
	va_end(aptr);

	LOG_M((const char*)buffer);

	return(ret);
}
int sprintf(char* buffer, const char* format, ...) {
	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vm_vsprintf(buffer, format, aptr);
	va_end(aptr);

	return(ret);
}
#endif