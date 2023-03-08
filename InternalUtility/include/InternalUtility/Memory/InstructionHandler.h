#pragma once
#include <vector>

#include <Log/Logger.h>
#include <InternalUtility/Memory/Typedefs.h>

namespace Hooking
{
	class RelativeInstructionHandler
	{
		struct RelativeInstructionType {
			RelativeInstructionType(const std::string& instructionName, const std::vector<byte>& bytes, const BaseType_t& offsetLength)
				: name(instructionName), bytes(bytes), offsetLength(offsetLength) {
			}

			std::string name;
			std::vector<byte> bytes;
			BaseType_t offsetLength;
		};

		struct RelativeInstruction
		{
			const RelativeInstructionType& instructionType;
			BaseType_t address;
		};

	public:
		RelativeInstructionHandler();

		bool relativeInstructionPresent(BaseType_t address);
		void patch4ByteOperand(BaseType_t originalAddress, BaseType_t newAddress);

	private:
		void registerInstruction(const std::string& instructionName, const std::vector<byte>& bytes, const size_t& offsetLength);
		int extractRelativeOffset(const RelativeInstruction& instruction);
		void setRelativeOffset(const RelativeInstruction& instruction, BaseType_t relativeOffset);
		const RelativeInstruction getRelativeInstruction(BaseType_t address);


	private:
		std::vector<RelativeInstructionType> m_relativeInstructionTypes;
	};

}