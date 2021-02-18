#include "NtWrapper.h"

#include <stdexcept>

typedef ULONG(__stdcall* tNtVirtualProtect) (HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG  NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection);
tNtVirtualProtect NtVirtualProtect;

void NtWrapper::init()
{
	auto ntHandle = GetModuleHandle("ntdll.dll");
	if (ntHandle) {
		auto address = (long)GetProcAddress(ntHandle, "NtProtectVirtualMemory");
		if (address) {
			NtVirtualProtect = (tNtVirtualProtect)address;
		}
	}
}

ULONG NtWrapper::virtualProtect(void* source, size_t length, ULONG flags, ULONG& returnValue)
{
	if (!NtVirtualProtect)
		throw std::runtime_error("Couldn't access NtProtectVirtualMemory");
	return NtVirtualProtect(((HANDLE)(LONG_PTR)-1), (void**)&source, (PULONG)&length, flags, &returnValue);
}
