#pragma once

#include <Windows.h>
#include <winternl.h>

namespace Memory
{
extern PEB* getPeb();
extern long getModuleBase();
}