#include "InternalUtility/Memory/InternalUtility.h"

namespace Internals
{
PEB* Internals::getPeb()
{
#ifdef _WIN64
	PEB* peb = (PEB*)__readgsqword(0x60);

#else
	PEB* peb = (PEB*)__readfsdword(0x30);
#endif

	return peb;
}

long Internals::getModuleBase()
{
	return (long)getPeb()->Reserved3[1];
}
}
