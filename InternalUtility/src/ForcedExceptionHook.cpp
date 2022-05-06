#include <stdexcept>
#include <InternalUtility/Memory/ForcedExceptionHook.h>
#include <Log/Logger.h>

#define EXCEPTION_VAL 0xFFFFFF00
#define EXCEPTION_VAL_COUNTER_MASK 0xFF

namespace Hook
{

void printRegisters(EXCEPTION_POINTERS* exceptionInfo)
{
	Log::Logger::Error("Code: 0x{:X}", exceptionInfo->ExceptionRecord->ExceptionCode);
	Log::Logger::Info("EIP: 0x{:X}", exceptionInfo->ContextRecord->Eip);
	Log::Logger::Warning("EBP: 0x{:X}", exceptionInfo->ContextRecord->Ebp);
	Log::Logger::Warning("ESI: 0x{:X}", exceptionInfo->ContextRecord->Esi);
	Log::Logger::Warning("EAX: 0x{:X}", exceptionInfo->ContextRecord->Eax);
	Log::Logger::Warning("EDX: 0x{:X}", exceptionInfo->ContextRecord->Edx);
	Log::Logger::Warning("ECX: 0x{:X}", exceptionInfo->ContextRecord->Ecx);
	Log::Logger::Warning("EDI: 0x{:X}", exceptionInfo->ContextRecord->Edi);
	Log::Logger::Warning("ESP: 0x{:X}", exceptionInfo->ContextRecord->Esp);
}

LONG WINAPI crashHandler(EXCEPTION_POINTERS* exceptionInfo)
{
	if (exceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION) {
		return EXCEPTION_CONTINUE_SEARCH;
	}

	bool fixed = false;
	for (auto& ctx: ForcedExceptionHook::hookContexts) {
		if (exceptionInfo->ContextRecord->Ebp == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->Ebp = ctx.restoreValue;
			fixed = true;
		}
		if (exceptionInfo->ContextRecord->Esi == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->Esi = ctx.restoreValue;			fixed = true;
		}
		if (exceptionInfo->ContextRecord->Eax == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->Eax = ctx.restoreValue;			fixed = true;
		}
		if (exceptionInfo->ContextRecord->Edx == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->Edx = ctx.restoreValue;			fixed = true;
		}
		if (exceptionInfo->ContextRecord->Ecx == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->Ecx = ctx.restoreValue;			fixed = true;
		}
		if (exceptionInfo->ContextRecord->Edi == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->Edi = ctx.restoreValue;			fixed = true;
		}
		if (exceptionInfo->ContextRecord->Esp == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->Esp = ctx.restoreValue;			fixed = true;
		}
		if (exceptionInfo->ContextRecord->Eip == ctx.expectedExceptionAddress) {
			ctx.callback(exceptionInfo);
		}
	}
	if (!fixed){
		Log::Logger::Fatal("Access violation can't be fixed!");
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
