#include "InternalUtility/Memory/Detour.h"
#include "WindowsWrapper/NtWrapper.h"
#include "Log/Logger.h"

namespace Memory
{
void* detour(unsigned char* src, const unsigned char* dst, const int len)
{
	ULONG retVal;
	BYTE* jmp = (BYTE*)malloc(len + 5);
	WindowsWrapper::NtVirtualProtect(src, len, PAGE_READWRITE, retVal);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	WindowsWrapper::NtVirtualProtect(src, len, retVal, retVal);
	WindowsWrapper::NtVirtualProtect(jmp - len, len + 5, PAGE_EXECUTE_READWRITE, retVal);
	return (jmp - len);
}

int DetourHook::relativeJumpPresent(void* address)
{
	byte* currentByte = (byte*)address;
	for (auto& relativeInstruction: m_relativeInstructions) {
		bool isRelative = true;
		for (int i = 0; i < relativeInstruction.length; i++) {
			isRelative &= currentByte[i] == relativeInstruction.instructionBytes[i];
		}
		if (isRelative) {
			Log::Logger::Info("Found {:s} instruction with relative address", relativeInstruction.name);
			return relativeInstruction.length;
		}
	}
	return false;
}

void fixRelativeOffset(void* originalAddress, void* newAddress, int numberOfBytes)
{
	byte* currentByte = (byte*)newAddress;
	Log::Logger::Debug("Patching relative address at 0x{:X}", (uint64_t)newAddress);
	int oldRelativeOffset = 0;
	// get original relative offset
	for (int idx = 0; idx < numberOfBytes; idx++) {
		oldRelativeOffset |= (currentByte[idx] & 0xFF) << (idx * 8);
	}
	uint64_t oldAbsoluteAddress = (uint64_t)originalAddress + numberOfBytes + oldRelativeOffset;
	Log::Logger::Debug("Relative function offset 0x{:X} points to 0x{:X}", oldRelativeOffset, oldAbsoluteAddress);

	int newRelativeCalOffs = oldAbsoluteAddress - ((uint64_t)currentByte + numberOfBytes);
	uint64_t newAbsoluteCallAdr = (uint64_t)currentByte + numberOfBytes + newRelativeCalOffs;
	Log::Logger::Debug("Relative function offset from instruction in trampoline mem 0x{:X} points to 0x{:X}", newRelativeCalOffs, newAbsoluteCallAdr);
	if (oldAbsoluteAddress != newAbsoluteCallAdr) {
		Log::Logger::Error("Cannot fix relative jump address! Relative offset points to wrong address. (Relative offset > 32 bit?)");
		return;
	}
	// write new relative offset
	for (int idx = 0; idx < numberOfBytes; idx++) {
		currentByte[idx] = (newRelativeCalOffs >> (idx * 8)) & 0xFF;
	}
}

void* DetourHook::trampoline64(void* src, void* dst, int len)
{
	Log::Logger::Info("Installing Hook at 0x{:X}", (uint64_t)src);
	int MinLen = 14;

	if (len < MinLen) {
		return nullptr;
	}

	BYTE stub[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp qword ptr [$+6]
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // ptr
	};

	//void* pTrampoline = VirtualAlloc(0, len + sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	void* pTrampoline = findCodeCave(src, len + sizeof(stub));
	Log::Logger::Info("Writing trampoline code to: 0x{:X}", (uint64_t)pTrampoline);
	DWORD tmp;
	VirtualProtect(pTrampoline, len + sizeof(stub), PAGE_EXECUTE_READWRITE, &tmp);

	ULONG dwOld = 0;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwOld);

	uint64_t returnAdrToOrig = (DWORD64)src + len;

	// trampoline
	memcpy(stub + 6, &returnAdrToOrig, 8);
	memcpy((void*)((uintptr_t)pTrampoline), src, len);
	memcpy((void*)((uintptr_t)pTrampoline + len), stub, sizeof(stub));

	Log::Logger::Debug("Returning to original code at 0x{:X}", returnAdrToOrig);

	Log::Logger::Info("Fixing relative addresses");
	// if there is a call instruction in the source bytes which are moved to the trampoline the relative offsets have to be recalculated
	for (int i = 0; i < len; i++) {
		BYTE* currentByte = (BYTE*)((uintptr_t)pTrampoline + i);
		int relativeJumpInstructionLength = relativeJumpPresent((void*)currentByte);
		if (relativeJumpInstructionLength) {
			auto origRelAdrLocation = (void*)((uintptr_t)src + i + relativeJumpInstructionLength);
			auto trampRelAdrLocation = (void*)&currentByte[relativeJumpInstructionLength];
			fixRelativeOffset(origRelAdrLocation, trampRelAdrLocation, 4);
		}
	}

	// orig
	memcpy(stub + 6, &dst, 8);
	memcpy(src, stub, sizeof(stub));

	for (int i = MinLen; i < len; i++) {
		*(BYTE*)((uintptr_t)src + i) = 0x90;
	}

	VirtualProtect(src, len, dwOld, &dwOld);
	Log::Logger::Info("Hook installed successfully");
	return (void*)((uintptr_t)pTrampoline);
}

DetourHook::DetourHook()
{
	byte callBytes[] = {0xE8};
	byte incBytes[] ={0xFF, 0x05};
	byte movBytes[] = {0xFF, 0x05};
	byte movBytes2[] = {0x48, 0x8b, 0x05};
	byte jnzBytes[] = {0x0F, 0x85};
	byte leaBytes[] = {0x48, 0x8D, 0x05};
	addRelativeInstruction("CALL", callBytes, 1);
	addRelativeInstruction("INC", incBytes, 2);
	addRelativeInstruction("MOV", movBytes, 2);
	addRelativeInstruction("MOV", movBytes2, 3);
	addRelativeInstruction("JNZ", jnzBytes, 2);
	addRelativeInstruction("LEA", leaBytes, 3);
}

void* DetourHook::findCodeCave(void* startAdr, int len)
{
	void* codeCave = nullptr;
	int count = 0;
	for (unsigned int i = 0; i < 2u * INT_MAX; i++) {
		byte* curByte = (byte*)((uint64_t)startAdr + i);
		if (curByte[0] == 0 && curByte[1] == 0) {
			count++;
		} else {
			count = 0;
		}
		if (count >= len) {
			codeCave = (void*)((uint64_t)(curByte) - len);
			break;
		}
	}
	return codeCave;
}

void DetourHook::addRelativeInstruction(const std::string& instructionName, byte instructionBytes[], int numBytes)
{
	m_relativeInstructions.emplace_back(instructionName, instructionBytes, numBytes);
}
}