#pragma once

#include <functional>
#include <vector>
#include <Windows.h>

namespace Hook
{

typedef std::function<void(EXCEPTION_POINTERS*)> FEHookCallback;

struct FEHookContext
{
	uint32_t exceptionValue;
	uint32_t pointerAddress;
	uint32_t restoreValue;
	uint32_t expectedExceptionAddress;
	bool debugOutput;
	FEHookCallback callback;
};

class ForcedExceptionHook
{
	public:
		static void initialize();
		static void addHook(uint32_t pointerAddress, uint32_t expectedExceptionAddress, const FEHookCallback& callback, bool debugOutput = false);

		inline static std::vector<FEHookContext> hookContexts;
};
}