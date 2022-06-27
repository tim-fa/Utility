#pragma once

#include <Windows.h>
#include <winternl.h>
#include <string>
#include <Psapi.h>

namespace Memory
{
extern PEB* getPeb();
extern uint64_t getModuleBase();
extern uint64_t offsetToModuleAddress(uint64_t offset);
extern MODULEINFO getModuleInfo(const char* moduleName);

}