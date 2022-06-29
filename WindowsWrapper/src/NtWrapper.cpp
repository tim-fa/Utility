#include "WindowsWrapper/NtWrapper.h"

#include <stdexcept>

static bool isInitialized = false;

typedef ULONG(__stdcall* tNtProtectVirtualMemory) (HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG  NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection);
tNtProtectVirtualMemory NtProtectVirtualMemory;

void init()
{
	auto ntHandle = GetModuleHandle("ntdll.dll");
	if (ntHandle) {
		auto address = (long)GetProcAddress(ntHandle, "NtProtectVirtualMemory");
		if (address) {
			NtProtectVirtualMemory = (tNtProtectVirtualMemory)address;
			isInitialized = true;
		}
	}
}

ULONG WindowsWrapper::NtVirtualProtect(void* source, size_t length, ULONG flags, ULONG& returnValue)
{
	if(!isInitialized)
		init();
	if (!NtProtectVirtualMemory)
		throw std::runtime_error("Couldn't access NtProtectVirtualMemory");
	return NtProtectVirtualMemory(((HANDLE)(LONG_PTR)-1), (void**)&source, (PULONG)&length, flags, &returnValue);
}
