#include <cstdint>
#include "InternalUtility/Memory/InternalUtility.h"

namespace Hooking
{
PEB* getPeb()
{
#ifdef __WIN32
	PEB* peb = (PEB*)__readfsdword(0x30);
#else
	PEB* peb = (PEB*)__readgsqword(0x60);
#endif
	return peb;
}

BaseType_t getModuleBase()
{
	return (BaseType_t)(size_t)getPeb()->Reserved3[1];
}

BaseType_t offsetToModuleAddress(BaseType_t offset)
{
	return getModuleBase() + offset;
}
MODULEINFO getModuleInfo(const char* moduleName)
{
	MODULEINFO modInfo = {nullptr};
	HMODULE hModule = GetModuleHandleA(moduleName);
	if (hModule == nullptr) {
		return modInfo;
	}
	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
	return modInfo;
}

}
