#pragma once
#include <InternalUtility/Memory/Typedefs.h>

namespace Hooking
{
	class CodeCave
	{
	public:
		CodeCave(BaseType_t startAddress, BaseType_t caveSize);

		BaseType_t writeData(byte* dataBuffer, BaseType_t size);

		BaseType_t getWriteOffset();
		BaseType_t getBaseAddress();
		BaseType_t getCurrentAddress();

	private:
		BaseType_t m_baseAddress{ 0 };
		BaseType_t m_size{ 0 };
		int m_writeOffset{ 0 };
	};

}