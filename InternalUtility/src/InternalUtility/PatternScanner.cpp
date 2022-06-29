#include "InternalUtility/Memory/PatternScanner.h"
#include <regex>

namespace Hooking
{
std::vector<short> stringToPatternBytes(const std::string& pattern)
{
	std::istringstream hex_chars_stream(pattern);
	std::vector<short> bytes;

	std::string c;
	while (hex_chars_stream >> c) {
		if (c.length() > 2) {
			throw std::runtime_error("Invalid pattern token: " + c);
		}
		if (c == "?") {
			bytes.push_back(-1);
		} else {
			bytes.push_back(strtol(c.c_str(), nullptr, 16) & 0xFF);
		}
	}
	return bytes;
}

BaseType_t findPattern(const std::string& pattern)
{
	std::regex patternRegex(R"(^(([0-9]|[a-f]|[A-F]){2}\s?|(\?\s?))*$)");
	std::smatch baseMatch;
	if (!std::regex_match(pattern, baseMatch, patternRegex)) {
		throw std::runtime_error("FindPattern(): Invalid pattern: " + pattern);
	}
	auto bytes = stringToPatternBytes(pattern);
	BaseType_t length = getModuleInfo(nullptr).SizeOfImage;
	for (BaseType_t adr = getModuleBase(); adr < getModuleBase() + length; adr++) {
		auto* currentByte = reinterpret_cast<unsigned char*>(adr);

		if (*currentByte == bytes[0]) {
			bool found = true;
			for (int idx = 0; idx < bytes.size(); idx++) {
				if (bytes[idx] != -1) {
					found &= bytes[idx] == currentByte[idx];
				}
			}
			if (found) {
				return adr;
			}
		}
	}
	throw std::runtime_error("Pattern not found: " + pattern);
}

BaseType_t findPlaceholderAddress(const std::string& pattern)
{
	std::regex patternRegex(R"(^(([0-9]|[a-f]|[A-F]){2}\s?|(\?\s?){4})*$)");
	std::smatch baseMatch;
	if (!std::regex_match(pattern, baseMatch, patternRegex)) {
		throw std::runtime_error("FindPlaceholderAddress(): Invalid pattern: " + pattern + ". Correct example: FF FF FF ? ? ? ? FF");
	}

	BaseType_t startAdr = 0;
	auto address = findPattern(pattern);
	auto bytes = stringToPatternBytes(pattern);
	for (int i = 0; i < bytes.size(); i++) {
		if (bytes[i] == -1) {
			startAdr = address + i;
			break;
		}
	}
	if (!startAdr) {
		throw std::runtime_error("Could not find pattern placeholder in pattern: " + pattern);
	}
	BaseType_t relativeAddress = 0;
	for (int i = 0; i < 4; i++) {
		relativeAddress |= reinterpret_cast<byte*>(startAdr)[i] << i * 8;
	}
	return relativeAddress + startAdr + 4;
}
}