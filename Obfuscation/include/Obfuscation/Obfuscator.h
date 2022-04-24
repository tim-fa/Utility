#include <iostream>
#include <format>
#include <random>


template <typename Type>
class Obfuscator {
public:
    Obfuscator(Type value, int numberOfSlots = 4)
            : bitStartIndex(new short[numberOfSlots])
            , data(new int[numberOfSlots])
            , bitMasks(new int[numberOfSlots])
            , numberOfSlots(numberOfSlots)
    {
        if (numberOfSlots > sizeof(Type) * 8) {
            throw std::runtime_error("Number of slots can not be bigger than number of bits");
        }
        typeSizeBits = sizeof(Type) * 8;
        bitsPerSlot = typeSizeBits / numberOfSlots;

        for (int maskIdx = 0; maskIdx < numberOfSlots; maskIdx++) {
            bitMasks[maskIdx] = 0;
            for (int i = 0; i < bitsPerSlot; i++) {
                bitMasks[maskIdx] |= (1 << (i + bitsPerSlot * maskIdx));
            }
        }
        set(value);
    }

    __forceinline void set(Type value) {
        static std::random_device dev;
        static std::mt19937 rng(dev());
        static std::uniform_int_distribution<std::mt19937::result_type> dist(1, INT_MAX);

        for (int i = 0; i < numberOfSlots; i++) {
            bitStartIndex[i] = dist(rng) % (sizeof(int) * 8 - bitsPerSlot);
            int bits = ~value & bitMasks[i];
            data[i] = dist(rng);
            data[i] &= ~(bitMasks[0] << bitStartIndex[i]);
            data[i] |= ((bits >> bitsPerSlot * i) << bitStartIndex[i]);
        }
    }

    __forceinline Type get()
    {
        Type returnValue = 0;
        for (int slotIdx = 0; slotIdx < numberOfSlots; slotIdx++) {
            returnValue |= (((~data[slotIdx] >> bitStartIndex[slotIdx]) & bitMasks[0]) << bitsPerSlot * slotIdx);
        }
        return returnValue;
    }

    __forceinline operator Type()
    {
        return get();
    }

    __forceinline Obfuscator<Type>& operator=(const Type& value) {
        set(value);
        return *this;
    }

    __forceinline Obfuscator<Type> operator+(const Type& value) {
        auto result = get() + value;
        return Obfuscator<Type>(result);
    }

    __forceinline Obfuscator<Type> operator-(const Type& value) {
        auto result = get() - value;
        return Obfuscator<Type>(result);
    }

    __forceinline Obfuscator<Type> operator++(Type) {
        auto result = get() + 1;
        set(result);
        return *this;
    }

    __forceinline Obfuscator<Type> operator--(Type) {
        auto result = get() - 1;
        set(result);
        return *this;
    }

private:
    int numberOfSlots;
    int typeSizeBits;
    int bitsPerSlot;
    std::shared_ptr<int[]> bitMasks;
    std::shared_ptr<short[]> bitStartIndex;
    std::shared_ptr<int[]> data;
};
