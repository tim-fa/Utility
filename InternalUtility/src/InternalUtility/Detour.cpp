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
		memcpy_s(jmp, len + 5, src, len);
		jmp += len;
		jmp[0] = 0xE9;
		*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
		src[0] = 0xE9;
		*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
		WindowsWrapper::NtVirtualProtect(src, len, retVal, retVal);
		WindowsWrapper::NtVirtualProtect(jmp - len, len + 5, PAGE_EXECUTE_READWRITE, retVal);
		return (jmp - len);
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
			if (m_instructionHandler.relativeInstructionPresent(addressToOriginalCode + i)) {
				m_instructionHandler.patch4ByteOperand((BaseType_t)hookAddress + i, addressToOriginalCode + i);
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


}