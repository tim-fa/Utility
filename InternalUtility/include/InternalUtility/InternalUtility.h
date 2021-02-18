#pragma once
#include <Windows.h>
#include <winternl.h>

namespace Internals {
    extern PEB* getPeb();
    extern long getModuleBase();

}