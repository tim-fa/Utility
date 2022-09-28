#pragma once
#include "HashedMemory.h"

namespace Obfuscation
{

template<typename Type>
class Obfuscator
{
	public:
		explicit Obfuscator();
		__forceinline void set(Type value);
		__forceinline Type get();
		__forceinline operator Type();
		__forceinline Obfuscator<Type>& operator=(const Type& value);
		__forceinline Obfuscator<Type> operator+(const Type& value);
		__forceinline Obfuscator<Type> operator-(const Type& value);
		__forceinline Obfuscator<Type> operator++(Type);
		__forceinline Obfuscator<Type> operator--(Type);

	private:
		HashedMemory memory;
};

}
