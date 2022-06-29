#pragma once

#include <Windows.h>
#include <winternl.h>
#include <string>
#include <Psapi.h>

#include "InternalUtility/Memory/Typedefs.h"

namespace Hooking
{
extern PEB* getPeb();
extern BaseType_t getModuleBase();
extern BaseType_t offsetToModuleAddress(BaseType_t offset);
extern MODULEINFO getModuleInfo(const char* moduleName);

}