#include <cstddef>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <Psapi.h>

#include "InternalUtility.h"
#include "Typedefs.h"

namespace Hooking
{
extern BaseType_t findPattern(const std::string& pattern);
extern BaseType_t findPlaceholderAddress(const std::string& pattern);
}