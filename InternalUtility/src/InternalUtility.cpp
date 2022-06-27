#include <cstdint>
#include "InternalUtility/Memory/InternalUtility.h"

namespace Memory
{
PEB* getPeb()
{
#ifdef _WIN64
	PEB* peb = (PEB*)__readgsqword(0x60);
#else
	PEB* peb = (PEB*)__readfsdword(0x30);
#endif

	return peb;
}

uint64_t getModuleBase()
{
	return (uint64_t)(size_t)getPeb()->Reserved3[1];
}

uint64_t offsetToModuleAddress(uint64_t offset)
{
	return getModuleBase() + offset;
}
MODULEINFO getModuleInfo(const char* moduleName)
{
	MODULEINFO modInfo = {nullptr};
	HMODULE hModule = GetModuleHandleA(moduleName);
	if (hModule == 0) {
		return modInfo;
	}
	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
	return modInfo;
}

}
