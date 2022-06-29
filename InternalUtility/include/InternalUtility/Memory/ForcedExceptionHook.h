#pragma once

#include <functional>
#include <vector>
#include <Windows.h>

#include "Typedefs.h"

namespace Hooking
{

typedef std::function<void(EXCEPTION_POINTERS*)> FEHookCallback;

struct FEHookContext
{
	BaseType_t exceptionValue;
	BaseType_t pointerAddress;
	BaseType_t restoreValue;
	BaseType_t expectedExceptionAddress;
	bool debugOutput;
	FEHookCallback callback;
};

class ForcedExceptionHook
{
	public:
		static void initialize();
		static void addHook(BaseType_t pointerAddress, BaseType_t expectedExceptionAddress, const FEHookCallback& callback, bool debugOutput = false);

		inline static std::vector<FEHookContext> hookContexts;
};
}