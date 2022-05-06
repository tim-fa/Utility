#include "InternalUtility/Memory/Detour.h"
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

void* Tramp64(void* src, void* dst, int len)
{
	int MinLen = 14;

	if (len < MinLen) return NULL;

	BYTE stub[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp qword ptr [$+6]
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // ptr
	};

	void* pTrampoline = VirtualAlloc(0, len + sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	ULONG dwOld = 0;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwOld);

	uintptr_t retto = (DWORD64)src + len;

	// trampoline
	memcpy(stub + 6, &retto, 8);
	memcpy((void*)((uintptr_t)pTrampoline), src, len);
	memcpy((void*)((uintptr_t)pTrampoline + len), stub, sizeof(stub));

	// orig
	memcpy(stub + 6, &dst, 8);
	memcpy(src, stub, sizeof(stub));

	for (int i = MinLen; i < len; i++)
	{
		*(BYTE*)((uintptr_t)src + i) = 0x90;
	}

	WindowsWrapper::NtVirtualProtect(src, len, dwOld, dwOld);
	return (void*)((uintptr_t)pTrampoline);
}
}