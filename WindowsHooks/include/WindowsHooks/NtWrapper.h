#pragma once
#include <Windows.h>

class NtWrapper
{
public:
	static void init();
	static ULONG virtualProtect(void* source, size_t length, ULONG flags, ULONG& returnValue);
};

