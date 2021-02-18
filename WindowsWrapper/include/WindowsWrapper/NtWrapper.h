#pragma once

#include <Windows.h>

namespace WindowsWrapper
{
extern ULONG NtVirtualProtect(void* source, size_t length, ULONG flags, ULONG& returnValue);
}

