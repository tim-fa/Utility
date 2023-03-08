#include <Log/Logger.h>
#include <InternalUtility/Memory/CodeCave.h>

#undef min

namespace Hooking
{
	CodeCave::CodeCave(BaseType_t startAddress, BaseType_t caveSize)
	{
		BaseType_t consecutiveFreeBytes = 0;

		for (BaseType_t byteIndex = 0; byteIndex < INT_MAX * 2U - 1; byteIndex++)
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
			if (consecutiveFreeBytes >= caveSize)
			{
				m_baseAddress = (startAddress + byteIndex) - caveSize;
				m_size = caveSize;
				break;
			}
		}
	}

	BaseType_t CodeCave::writeData(byte* dataBuffer, BaseType_t size)
	{
		BaseType_t startAddress = getCurrentAddress();
		BaseType_t remainingSize = m_size - m_writeOffset;

		if (remainingSize < size)
		{
			throw std::length_error("Code cave is too small!");
		}
		Log::Logger::Debug("Writing {:d} bytes at index {:d} ({:d} bytes free)", size, m_writeOffset, remainingSize);
		memcpy_s((void*)(m_baseAddress + m_writeOffset), (m_size - m_writeOffset), dataBuffer, size);
		m_writeOffset += size;
		return startAddress;
	}

	BaseType_t CodeCave::getWriteOffset()
	{
		return m_baseAddress + m_writeOffset;
	}

	BaseType_t CodeCave::getBaseAddress()
	{
		return m_baseAddress;
	}

	BaseType_t CodeCave::getCurrentAddress()
	{
		return m_baseAddress + m_writeOffset;
	}
}