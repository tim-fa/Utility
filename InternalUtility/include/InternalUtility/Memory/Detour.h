#pragma once

#ifndef DETOUR_H
#define DETOUR_H

#include <Windows.h>
#include <string>
#include <memory>
#include <vector>

namespace Hook
{
extern void* detour(unsigned char* src, const unsigned char* dst, int len);

class DetourHook
{
		struct RelativeInstruction
		{
			RelativeInstruction(const std::string& instructionName, byte bytes[], int numBytes)
			{
				name = instructionName;
				instructionBytes = std::shared_ptr<byte[]>(new byte[numBytes]);
				memcpy_s(instructionBytes.get(), numBytes, bytes, numBytes);
				length = numBytes;
			}

			std::string name;
			std::shared_ptr<byte[]> instructionBytes;
			int length;
		};

	public:

		DetourHook();
		void* trampoline64(void* src, void* dst, int len);

	private:
		void* findCodeCave(void* startAdr, int len);
		int relativeJumpPresent(void* address);
		void addRelativeInstruction(const std::string& instructionName, byte instructionBytes[], int numBytes);

		std::vector<RelativeInstruction> m_relativeInstructions;
};
}

#endif
