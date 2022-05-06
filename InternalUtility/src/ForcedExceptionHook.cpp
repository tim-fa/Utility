#include <stdexcept>
#include <InternalUtility/Memory/ForcedExceptionHook.h>
#include <Log/Logger.h>

#define EXCEPTION_VAL 0xFFFFFF00
#define EXCEPTION_VAL_COUNTER_MASK 0xFF

namespace Hook
{

LONG WINAPI crashHandler(EXCEPTION_POINTERS* exceptionInfo)
{
	if (exceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION) {
		return EXCEPTION_CONTINUE_SEARCH;
	}

	for (auto& ctx: ForcedExceptionHook::hookContexts) {
		if (exceptionInfo->ContextRecord->Ebp == ctx.exceptionValue) {
			exceptionInfo->ContextRecord->Ebp = ctx.restoreValue;
		} else if (exceptionInfo->ContextRecord->Esi == ctx.exceptionValue) {
			exceptionInfo->ContextRecord->Esi = ctx.restoreValue;
		} else if (exceptionInfo->ContextRecord->Eax == ctx.exceptionValue) {
			exceptionInfo->ContextRecord->Eax = ctx.restoreValue;
		} else if (exceptionInfo->ContextRecord->Edx == ctx.exceptionValue) {
			exceptionInfo->ContextRecord->Edx = ctx.restoreValue;
		} else if (exceptionInfo->ContextRecord->Ecx == ctx.exceptionValue) {
			exceptionInfo->ContextRecord->Ecx = ctx.restoreValue;
		} else if (exceptionInfo->ContextRecord->Edi == ctx.exceptionValue) {
			exceptionInfo->ContextRecord->Edi = ctx.restoreValue;
		} else if (exceptionInfo->ContextRecord->Esp == ctx.exceptionValue) {
			exceptionInfo->ContextRecord->Esp = ctx.restoreValue;
		} else {
			Log::Logger::Fatal("Could not restore from exception!");
			exit(-1);
		}
		if (exceptionInfo->ContextRecord->Eip == ctx.expectedExceptionAddress) {
			ctx.callback(exceptionInfo);
		}

		if (ctx.debugOutput) {
			Log::Logger::Debug("Code: 0x{:X}", exceptionInfo->ExceptionRecord->ExceptionCode);
			Log::Logger::Debug("EIP: 0x{:X}", exceptionInfo->ContextRecord->Eip);
			Log::Logger::Debug("EBP: 0x{:X}", exceptionInfo->ContextRecord->Ebp);
			Log::Logger::Debug("ESI: 0x{:X}", exceptionInfo->ContextRecord->Esi);
			Log::Logger::Debug("EAX: 0x{:X}", exceptionInfo->ContextRecord->Eax);
			Log::Logger::Debug("EDX: 0x{:X}", exceptionInfo->ContextRecord->Edx);
			Log::Logger::Debug("EXC: 0x{:X}", exceptionInfo->ContextRecord->Ecx);
			Log::Logger::Debug("EDI: 0x{:X}", exceptionInfo->ContextRecord->Edi);
			Log::Logger::Debug("ESP: 0x{:X}", exceptionInfo->ContextRecord->Esp);
		}
		break;
	}
	return EXCEPTION_CONTINUE_EXECUTION;
}

void ForcedExceptionHook::initialize()
{
	AddVectoredExceptionHandler(1, crashHandler);
}

void ForcedExceptionHook::addHook(uint32_t pointerAddress, uint32_t expectedExceptionAddress, const FEHookCallback& callback, bool debugOutput)
{
	if (hookContexts.size() >= EXCEPTION_VAL_COUNTER_MASK) {
		throw std::runtime_error("Maximal number of hooks reached!");
	}
	FEHookContext ctx;
	uint32_t exceptionValue = EXCEPTION_VAL | (hookContexts.size() & EXCEPTION_VAL_COUNTER_MASK);

	ctx.pointerAddress = pointerAddress;
	ctx.restoreValue = *(uint32_t*)pointerAddress;
	ctx.callback = callback;
	ctx.exceptionValue = exceptionValue;
	ctx.expectedExceptionAddress = expectedExceptionAddress;
	ctx.debugOutput = debugOutput;

	hookContexts.push_back(ctx);
	*(uint32_t*)ctx.pointerAddress = ctx.exceptionValue;

	if (ctx.debugOutput) {
		Log::Logger::Debug("Address: 0x{:X}", ctx.pointerAddress);
		Log::Logger::Debug("Restore Value: 0x{:X}", ctx.restoreValue);
		Log::Logger::Debug("Exception Value: 0x{:X}", ctx.exceptionValue);
		Log::Logger::Debug("Expected Exception Address: 0x{:X}", ctx.expectedExceptionAddress);
	}
}
}
