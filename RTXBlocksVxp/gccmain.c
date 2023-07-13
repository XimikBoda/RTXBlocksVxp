#include <stdio.h>
#include "vmsys.h"
#include "vmchset.h"
#include "vmlog.h"
#include "vmio.h"

typedef VMINT(*vm_get_sym_entry_t)(char* symbol);
vm_get_sym_entry_t vm_get_sym_entry;
typedef void (**__init_array) (void);

int __g_errno = 0;

void __cxa_pure_virtual()
{
	while (1);
}

int __cxa_guard_acquire(int* p)
{
	return 1;
}

void __cxa_guard_release(int* p)
{
	return;
}

int* __errno()
{
	return &__g_errno;
}

void gcc_entry(unsigned int entry, unsigned int init_array_start, unsigned int count)
{
	unsigned int i;
	__init_array ptr;
	vm_get_sym_entry = (vm_get_sym_entry_t)entry;
	ptr = (__init_array)init_array_start;
	for (i = 1; i < count; i++)
	{
		ptr[i]();
	}
	vm_main();
}
