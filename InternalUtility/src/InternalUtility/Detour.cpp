#include <InternalUtility/Memory/Typedefs.h>
#include <InternalUtility/Memory/Detour.h>
#include <WindowsWrapper/NtWrapper.h>
#include <Log/Logger.h>


struct CodeCave
{
	BaseType_t startAddress;
	BaseType_t size;
};

class CodeCaveManager
{
public:

	static CodeCave findCodeCave(BaseType_t startAddress, size_t size)
	{
		CodeCave codeCave{ 0, 0 };
		size_t consecutiveFreeBytes = 0;

		for (size_t byteIndex = 0; byteIndex < INT_MAX * 2U - 1; byteIndex++)
		{
			byte currentByte = *(byte*)(startAddress + byteIndex);
			byte nextByte = *(byte*)(startAddress + byteIndex + 1);
			if (currentByte == 0 && nextByte == 0)
			{
				consecutiveFreeBytes++;
			}
			else
			{
				consecutiveFreeBytes = 0;
			}
			if (consecutiveFreeBytes >= size)
			{
				codeCave.startAddress = (startAddress + byteIndex) - size;
				codeCave.size = size;
				break;
			}
		}
		return codeCave;
	}

	static void writeData(const CodeCave& codeCave, byte* dataBuffer, size_t size)
	{
		if (size > codeCave.size) {
			Log::Logger::Warning("Code cave is too small for data! {:d} bytes will be lost!", size - codeCave.size);
		}
		memcpy_s((void*)codeCave.startAddress, codeCave.size, dataBuffer, size);
	}


};





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

	void* DetourHook::trampoline64(void* src, void* dst, int len)
	{
		Log::Logger::Info("Installing Hook at 0x{:X}", (BaseType_t)src);
		int MinLen = 14;

		if (len < MinLen) {
			return nullptr;
		}

		BYTE stub[] = {
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp qword ptr [$+6]
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // ptr
		};

		//void* pTrampoline = VirtualAlloc(0, len + sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);


		CodeCave cc = CodeCaveManager::findCodeCave((BaseType_t)src, len + sizeof(stub));

		Log::Logger::Info("Writing trampoline code to: 0x{:X}", cc.startAddress);
		DWORD tmp;
		VirtualProtect((void*)cc.startAddress, len + sizeof(stub), PAGE_EXECUTE_READWRITE, &tmp);

		ULONG dwOld = 0;
		VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwOld);

		BaseType_t returnAdrToOrig = (BaseType_t)src + len;

		// trampoline
		memcpy(stub + 6, &returnAdrToOrig, 8);
		memcpy((void*)cc.startAddress, src, len);
		memcpy((void*)(cc.startAddress + len), stub, sizeof(stub));

		Log::Logger::Debug("Returning to original code at 0x{:X}", returnAdrToOrig);

		Log::Logger::Info("Fixing relative addresses");
		// if there is a call instruction in the source bytes which are moved to the trampoline the relative offsets have to be recalculated
		for (int i = 0; i < len; i++) {
			BYTE* currentByte = (BYTE*)(cc.startAddress + i);
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
		return (void*)cc.startAddress;
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