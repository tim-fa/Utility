#include <InternalUtility/Memory/Typedefs.h>
#include <Log/Logger.h>

#include <InternalUtility/Memory/InstructionHandler.h>

#undef min

namespace Hooking
{
	RelativeInstructionHandler::RelativeInstructionHandler()
	{
		registerInstruction("CALL", { 0xE8 }, 4);
		registerInstruction("INC", { 0xFF, 0x05 }, 4);
		registerInstruction("MOV", { 0xFF, 0x05 }, 4);
		registerInstruction("MOV", { 0x48, 0x8b, 0x05 }, 4);
		registerInstruction("JNZ", { 0x0F, 0x85 }, 4);
		registerInstruction("LEA", { 0x48, 0x8D, 0x05 }, 4);
	}

	bool RelativeInstructionHandler::relativeInstructionPresent(BaseType_t address)
	{
		byte* data = (byte*)address;
		for (auto& rit : m_relativeInstructionTypes) {
			bool isRelative = true;
			for (int i = 0; i < rit.bytes.size(); i++) {
				isRelative &= data[i] == rit.bytes[i];
			}
			if (isRelative) {
				return true;
			}
		}
		return false;
	}

	void RelativeInstructionHandler::patch4ByteOperand(BaseType_t originalAddress, BaseType_t newAddress)
	{
		try
		{
			RelativeInstruction relativeInstruction = getRelativeInstruction(originalAddress);
			Log::Logger::Debug("==== Patching {:s} instruction ====", relativeInstruction.instructionType.name);

			int originalRelativeOffset = extractRelativeOffset(relativeInstruction);
			BaseType_t expectedTargetAddress = originalAddress + relativeInstruction.instructionType.offsetLength + originalRelativeOffset;
			int patchedRelativeOffset = (int)(expectedTargetAddress - (newAddress + relativeInstruction.instructionType.offsetLength));
			BaseType_t patchedTargetAddress = newAddress + relativeInstruction.instructionType.offsetLength + patchedRelativeOffset;

			if (patchedTargetAddress != expectedTargetAddress) {
				Log::Logger::Error("Cannot fix relative jump address! Relative offset points to wrong address. (Relative offset > 32 bit?)");
				return;
			}

			relativeInstruction.address = newAddress;
			setRelativeOffset(relativeInstruction, patchedRelativeOffset);


			Log::Logger::Debug("* Summary");
			Log::Logger::Debug("* Original offset: 0x{:X}", (BaseType_t)originalRelativeOffset);
			Log::Logger::Debug("* Patched offset: 0x{:X}", (BaseType_t)patchedRelativeOffset);
			Log::Logger::Debug("* Target address: 0x{:X}", expectedTargetAddress);
		}
		catch (std::exception& e)
		{
			Log::Logger::Warning(e.what());
		}
	}


	void RelativeInstructionHandler::registerInstruction(const std::string& instructionName, const std::vector<byte>& bytes, const size_t& offsetLength)
	{
		m_relativeInstructionTypes.emplace_back(instructionName, bytes, offsetLength);
	}

	int RelativeInstructionHandler::extractRelativeOffset(const RelativeInstruction& instruction)
	{
		int relativeOffset = 0;
		for (BaseType_t byteIndex = 0; byteIndex < instruction.instructionType.offsetLength; byteIndex++) {
			relativeOffset |= (*(byte*)(instruction.address + instruction.instructionType.bytes.size() + byteIndex) & 0xFF) << (byteIndex * 8);
		}
		return relativeOffset;
	}

	void RelativeInstructionHandler::setRelativeOffset(const RelativeInstruction& instruction, BaseType_t relativeOffset)
	{
		for (BaseType_t byteIndex = 0; byteIndex < instruction.instructionType.offsetLength; byteIndex++) {
			*(byte*)(instruction.address + instruction.instructionType.bytes.size() + byteIndex) = (relativeOffset >> (byteIndex * 8)) & 0xFF;
		}
	}

	const RelativeInstructionHandler::RelativeInstruction RelativeInstructionHandler::getRelativeInstruction(BaseType_t address)
	{
		byte* data = (byte*)address;
		for (auto& rit : m_relativeInstructionTypes) {
			bool isRelative = true;
			for (int i = 0; i < rit.bytes.size(); i++) {
				isRelative &= data[i] == rit.bytes[i];
			}
			if (isRelative) {
				return RelativeInstruction{ rit, address };
			}
		}
		throw std::runtime_error("No relative instruction was found!");
	}
}