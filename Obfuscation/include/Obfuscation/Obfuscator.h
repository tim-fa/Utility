#include <iostream>
#include <format>
#include <random>
#include <sstream>
#include "HashedMemory.h"

namespace Obfuscation
{


template<typename Type>
class Obfuscator
{
	public:
		explicit Obfuscator(Type value = 0)
			: memory(sizeof(Type))
		{
		}

		__forceinline void set(Type value)
		{
			memory.write(reinterpret_cast<unsigned char*>(&value));
		}

		__forceinline Type get()
		{
			return *(Type*)memory.read().get();
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
		HashedMemory memory;
};
}
