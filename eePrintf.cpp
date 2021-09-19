#include "eePrintf.hpp"

void eePuts(const char* str)
{
	const size_t size = strlen(str);
	volatile char* const ee_debug_out = (volatile char*)0x1000F180;

	for (int i = 0; i < size; i++)
	{
		*ee_debug_out = str[i];
	}
	return;
}

void eePrintf(const char* fmt, ...)
{
	char str[256];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
	va_end(args);
	printf(str);
	eePuts(str);
}