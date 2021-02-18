#include "InternalUtility/Detour.h"
#include "WindowsWrapper/NtWrapper.h"

namespace Internals
{
void* detour(unsigned char* src, const unsigned char* dst, const int len)
{
	ULONG retVal;
	BYTE* jmp = (BYTE*)malloc(len + 5);
	WindowsWrapper::NtVirtualProtect(src, len, PAGE_READWRITE, retVal);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	WindowsWrapper::NtVirtualProtect(src, len, retVal, retVal);
	WindowsWrapper::NtVirtualProtect(jmp - len, len + 5, PAGE_EXECUTE_READWRITE, retVal);
	return (jmp - len);
}
}