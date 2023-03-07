#include <InternalUtility/Memory/Typedefs.h>
#include <InternalUtility/Memory/Detour.h>
#include <InternalUtility/Memory/CodeCave.h>
#include <WindowsWrapper/NtWrapper.h>
#include <Log/Logger.h>


namespace Hooking
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
		for (auto& relativeInstruction : m_relativeInstructions) {
			bool isRelative = true;
			for (int i = 0; i < relativeInstruction.bytes.size(); i++) {
				isRelative &= currentByte[i] == relativeInstruction.bytes[i];
			}
			if (isRelative) {
				Log::Logger::Info("Found {:d}-byte '{:s}' instruction with relative address", relativeInstruction.bytes.size(), relativeInstruction.name);
				return static_cast<int>(relativeInstruction.bytes.size());
			}
		}
		return false;
	}

	void fixRelativeOffset(void* originalAddress, void* newAddress, int numberOfBytes)
	{
		byte* currentByte = (byte*)newAddress;
		Log::Logger::Debug("Patching relative address at 0x{:X}", (BaseType_t)newAddress);
		int oldRelativeOffset = 0;
		// get original relative offset
		for (int idx = 0; idx < numberOfBytes; idx++) {
			oldRelativeOffset |= (currentByte[idx] & 0xFF) << (idx * 8);
		}
		BaseType_t oldAbsoluteAddress = (BaseType_t)originalAddress + numberOfBytes + oldRelativeOffset;
		Log::Logger::Debug("Relative function offset 0x{:X} points to 0x{:X}", oldRelativeOffset, oldAbsoluteAddress);

		int newRelativeCalOffs = oldAbsoluteAddress - ((BaseType_t)currentByte + numberOfBytes);
		BaseType_t newAbsoluteCallAdr = (BaseType_t)currentByte + numberOfBytes + newRelativeCalOffs;
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

	void* DetourHook::trampoline64(byte* hookAddress, void* dst, int len)
	{
		Log::Logger::Info("Installing Hook at 0x{:X}", (BaseType_t)hookAddress);
		int MinLen = 14;

		if (len < MinLen) {
			return nullptr;
		}

		BYTE stub[] = {
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp qword ptr [$+6]
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // ptr
		};

		//void* pTrampoline = VirtualAlloc(0, len + sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);


		CodeCave cc((BaseType_t)hookAddress, len + sizeof(stub));

		Log::Logger::Info("Writing trampoline code to: 0x{:X}", cc.getBaseAddress());
		DWORD tmp;
		VirtualProtect((void*)cc.getBaseAddress(), len + sizeof(stub), PAGE_EXECUTE_READWRITE, &tmp);

		ULONG dwOld = 0;
		VirtualProtect(hookAddress, len, PAGE_EXECUTE_READWRITE, &dwOld);

		BaseType_t returnAdrToOrig = (BaseType_t)hookAddress + len;

		// trampoline
		memcpy(stub + 6, &returnAdrToOrig, 8);

		BaseType_t addressToOriginalCode = cc.writeData((byte*)hookAddress, len);
		cc.writeData(stub, sizeof(stub));

		Log::Logger::Debug("Returning to original code at 0x{:X}", returnAdrToOrig);

		Log::Logger::Info("Fixing relative addresses");
		// if there is a call instruction in the source bytes which are moved to the trampoline the relative offsets have to be recalculated
		for (int i = 0; i < len; i++) {
			BYTE* currentByte = (BYTE*)(addressToOriginalCode + i);
			int relativeJumpInstructionLength = relativeJumpPresent((void*)currentByte);
			if (relativeJumpInstructionLength) {
				auto origRelAdrLocation = (void*)((uintptr_t)hookAddress + i + relativeJumpInstructionLength);
				auto trampRelAdrLocation = (void*)&currentByte[relativeJumpInstructionLength];
				fixRelativeOffset(origRelAdrLocation, trampRelAdrLocation, 4);
			}
		}

		// orig
		memcpy(stub + 6, &dst, 8);
		memcpy(hookAddress, stub, sizeof(stub));

		for (int i = MinLen; i < len; i++) {
			*(BYTE*)((uintptr_t)hookAddress + i) = 0x90;
		}

		VirtualProtect(hookAddress, len, dwOld, &dwOld);
		Log::Logger::Info("Hook installed successfully");
		return (void*)addressToOriginalCode;
	}

	DetourHook::DetourHook()
	{
		addRelativeInstruction("CALL", { 0xE8 });
		addRelativeInstruction("INC", { 0xFF, 0x05 });
		addRelativeInstruction("MOV", { 0xFF, 0x05 });
		addRelativeInstruction("MOV", { 0x48, 0x8b, 0x05 });
		addRelativeInstruction("JNZ", { 0x0F, 0x85 });
		addRelativeInstruction("LEA", { 0x48, 0x8D, 0x05 });
	}


	void DetourHook::addRelativeInstruction(const std::string& instructionName, const std::vector<byte>& bytes)
	{
		m_relativeInstructions.emplace_back(instructionName, bytes);
	}
}