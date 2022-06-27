#include <cstddef>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <Psapi.h>

#include "InternalUtility.h"

namespace Memory
{
extern uint64_t findPattern(const std::string& pattern);
extern uint64_t findPlaceholderAddress(const std::string& pattern);
}