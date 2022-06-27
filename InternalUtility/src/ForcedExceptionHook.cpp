#include <stdexcept>
#include <InternalUtility/Memory/ForcedExceptionHook.h>
#include <Log/Logger.h>

#define EXCEPTION_VAL 0xFFFFFF00
#define EXCEPTION_VAL_COUNTER_MASK 0xFF

#ifdef AAA
#define EAX Eax
#define EDX Edx
#define ECX Ecx
#define ESP Esp
#define ESI Esi
#define EIP Eip
#define EBP Ebp
#define EDI Edi
#else
#define EAX Rax
#define EDX Rdx
#define ECX Rcx
#define ESP Rsp
#define ESI Rsi
#define EIP Rip
#define EBP Rbp
#define EDI Rdi
#endif

namespace Memory
{

void printRegisters(EXCEPTION_POINTERS* exceptionInfo)
{
	Log::Logger::Error("Code: 0x{:X}", exceptionInfo->ExceptionRecord->ExceptionCode);
	Log::Logger::Info("EIP: 0x{:X}", exceptionInfo->ContextRecord->EIP);
	Log::Logger::Warning("EBP: 0x{:X}", exceptionInfo->ContextRecord->EBP);
	Log::Logger::Warning("ESI: 0x{:X}", exceptionInfo->ContextRecord->ESI);
	Log::Logger::Warning("EAX: 0x{:X}", exceptionInfo->ContextRecord->EAX);
	Log::Logger::Warning("EDX: 0x{:X}", exceptionInfo->ContextRecord->EDX);
	Log::Logger::Warning("ECX: 0x{:X}", exceptionInfo->ContextRecord->ECX);
	Log::Logger::Warning("EDI: 0x{:X}", exceptionInfo->ContextRecord->EDI);
	Log::Logger::Warning("ESP: 0x{:X}", exceptionInfo->ContextRecord->ESP);
}

LONG WINAPI crashHandler(EXCEPTION_POINTERS* exceptionInfo)
{
	if (exceptionInfo->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION) {
		return EXCEPTION_CONTINUE_SEARCH;
	}
	printRegisters(exceptionInfo);
	getchar();
	bool fixed = false;
	for (auto& ctx: ForcedExceptionHook::hookContexts) {
		if (exceptionInfo->ContextRecord->EBP == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->EBP = ctx.restoreValue;
			fixed = true;
		}
		if (exceptionInfo->ContextRecord->ESI == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->ESI = ctx.restoreValue;
			fixed = true;
		}
		if (exceptionInfo->ContextRecord->EAX == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->EAX = ctx.restoreValue;
			fixed = true;
		}
		if (exceptionInfo->ContextRecord->EDX == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->EDX = ctx.restoreValue;
			fixed = true;
		}
		if (exceptionInfo->ContextRecord->ECX == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->ECX = ctx.restoreValue;
			fixed = true;
		}
		if (exceptionInfo->ContextRecord->EDI == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->EDI = ctx.restoreValue;
			fixed = true;
		}
		if (exceptionInfo->ContextRecord->ESP == ctx.exceptionValue) {
			if (ctx.debugOutput) {
				printRegisters(exceptionInfo);
			}
			exceptionInfo->ContextRecord->ESP = ctx.restoreValue;
			fixed = true;
		}
		if (exceptionInfo->ContextRecord->EIP == ctx.expectedExceptionAddress) {
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
