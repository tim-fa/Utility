#include <iostream>
#include <format>
#include <random>
#include <sstream>

namespace Obfuscation
{
enum SplitCount : int
{
	Split2 = 2,
	Split4 = 4,
	Split8 = 8
};

int randomInt()
{
	static std::random_device dev;
	static std::mt19937 rng(dev());
	static std::uniform_int_distribution<std::mt19937::result_type> dist(1, 2 * INT_MAX);
	return dist(rng);
}

template<typename Type>
class Obfuscator
{
	public:
		explicit Obfuscator(Type value = 0, SplitCount splitCount = SplitCount::Split2)
			: bitMasks(new int[splitCount])
			, bitStartIndex(new short[splitCount])
			, data(new int[splitCount])
			, splitCount(splitCount)
		{
			verificationHash = randomInt();

			int varBits = sizeof(Type) * 8;
			if (splitCount > varBits) {
				throw std::runtime_error("A " + std::to_string(varBits) + "-Bit Variable can not be split in to more than " + std::to_string(varBits) + " individual Variables");
			}
			bitsPerSlot =  sizeof(Type) * 8 / splitCount;

			for (int maskIdx = 0; maskIdx < splitCount; maskIdx++) {
				bitMasks[maskIdx] = 0;
				for (int i = 0; i < bitsPerSlot; i++) {
					bitMasks[maskIdx] |= (1 << (i + bitsPerSlot * maskIdx));
				}
			}
			set(value);
		}

		__forceinline void set(Type value)
		{
			static std::hash<unsigned int> bitsHash;
			for (int i = 0; i < splitCount; i++) {
				bitStartIndex[i] = randomInt() % (sizeof(int) * 8 - bitsPerSlot);
				unsigned int bitsToSetMask = bitMasks[0] << bitStartIndex[i];
				unsigned int bitsToSet = ~value & bitMasks[i];
				// Set the data bits at the correct location
				data[i] = ((bitsToSet >> bitsPerSlot * i) << bitStartIndex[i]);
				unsigned int rawBitsAlignedRight = value >> (bitsPerSlot * i) & bitMasks[0];
				auto bitHashValue = static_cast<unsigned int>(bitsHash(rawBitsAlignedRight));
				data[i] |= (verificationHash ^ bitHashValue) & ~bitsToSetMask;
			}
		}

		__forceinline Type get()
		{
			static std::hash<unsigned int> bitsHash;
			Type returnValue = 0;
			for (int slotIdx = 0; slotIdx < splitCount; slotIdx++) {
				unsigned int dataBits = (~data[slotIdx] >> bitStartIndex[slotIdx]) & bitMasks[0];
				unsigned int readBitsMask = bitMasks[0] << bitStartIndex[slotIdx];
				unsigned int actualHash = data[slotIdx] & ~readBitsMask;
				unsigned int calculatedHash = (verificationHash ^ static_cast<unsigned int>(bitsHash(dataBits))) & ~readBitsMask;
				if (calculatedHash != actualHash) {
					throw std::runtime_error("Value was modified illegally");
				}
				returnValue |= dataBits << bitsPerSlot * slotIdx;
			}
			return returnValue;
		}

		__forceinline operator Type()
		{
			return get();
		}

		__forceinline Obfuscator<Type>& operator=(const Type& value)
		{
			set(value);
			return *this;
		}

		__forceinline Obfuscator<Type> operator+(const Type& value)
		{
			auto result = get() + value;
			return Obfuscator<Type>(result);
		}

		__forceinline Obfuscator<Type> operator-(const Type& value)
		{
			auto result = get() - value;
			return Obfuscator<Type>(result);
		}

		__forceinline Obfuscator<Type> operator++(Type)
		{
			auto result = get() + 1;
			set(result);
			return *this;
		}

		__forceinline Obfuscator<Type> operator--(Type)
		{
			auto result = get() - 1;
			set(result);
			return *this;
		}

	private:
		SplitCount splitCount;
		int bitsPerSlot;
		int verificationHash;
		std::shared_ptr<int[]> bitMasks;
		std::shared_ptr<short[]> bitStartIndex;
		std::shared_ptr<int[]> data;
};
}
