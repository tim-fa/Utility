#pragma once

#ifndef ADDRESS_PTR_H
#define ADDRESS_PTR_H

#include <vector>
#include <stdexcept>
#include <exception>
#include "InternalUtility.h"

#undef __try

namespace Memory
{
template<typename Type = long, long TypeSize = 0>
class Variable
{
	public:
		Variable() = default;

		template<typename... Args>
		explicit Variable(Args... offsets);

		template<typename... Args>
		void initialize(Args... offsets);

		void printPointerPath();

		operator Type&();

		Variable<Type, TypeSize>& operator=(Type value);

		Type operator&=(Type value);

		Type* operator&();

		Type* operator->();

		Type& operator[](int index);

		long getAddress();

		void setValue(Type value);

	private:
		void initializePointer();

		void addOffset(long offset);

		long currentAddress{0};
		std::vector<int> offs;
};

template<typename Type, long TypeSize>
template<typename... Args>
inline Variable<Type, TypeSize>::Variable(Args... offsets)
	: offs({offsets...})
{
}

template<typename Type, long TypeSize>
template<typename... Args>
inline void Variable<Type, TypeSize>::initialize(Args... offsets)
{
	for (auto& offset: {offsets...}) {
		offs.push_back(offset);
	}
}

template<typename Type, long TypeSize>
inline void Variable<Type, TypeSize>::printPointerPath()
{
	long curAdr = Memory::getModuleBase();
	int i = 0;
	for (auto& offset: offs) {
		long pointedValue = *(long*)(curAdr + offset);
		printf("Level %2d: 0x%010X + 0x%05X [0x%010X] -> 0x%X (Decimal: %d)\n", i++, curAdr, offset,
			curAdr + offset, pointedValue,
			pointedValue);
		curAdr = pointedValue;
	}
}

template<typename Type, long TypeSize>
inline void Variable<Type, TypeSize>::addOffset(long offset)
{
	offs.push_back(offset);
}

template<typename Type, long TypeSize>
inline Variable<Type, TypeSize>::operator Type&()
{
	return *(Type*)getAddress();
}

template<typename Type, long TypeSize>
inline Variable<Type, TypeSize>& Variable<Type, TypeSize>::operator=(Type value)
{
	setValue(value);
	return *this;
}

template<typename Type, long TypeSize>
inline Type* Variable<Type, TypeSize>::operator->()
{
	return (Type*)getAddress();
}

template<typename Type, long TypeSize>
inline Type* Variable<Type, TypeSize>::operator&()
{
	return (Type*)getAddress();
}

template<typename Type, long TypeSize>
inline Type& Variable<Type, TypeSize>::operator[](int index)
{
	if (!TypeSize) {
		throw std::runtime_error("Type size is 0!");
	}
	return *(Type*)(getAddress() + TypeSize * index);
}

template<typename Type, long TypeSize>
long Variable<Type, TypeSize>::getAddress()
{
	initializePointer();
	return currentAddress;
}

template<typename Type, long TypeSize>
void Variable<Type, TypeSize>::setValue(Type value)
{
	initializePointer();
	*(Type*)currentAddress = value;
}

template<typename Type, long TypeSize>
void Variable<Type, TypeSize>::initializePointer()
{
	__try{
		currentAddress = Memory::getModuleBase();
		bool first = true;
		for (auto& offset : offs) {
			if (!first) {
				currentAddress = *(long*)currentAddress;
			}
			currentAddress = currentAddress + offset;
			first = false;
		}
		*(long*)currentAddress++;
		*(long*)currentAddress--;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		throw std::runtime_error("Access violation! Address is not (yet) accessible");
	}
}

template<typename Type, long TypeSize>
Type Variable<Type, TypeSize>::operator&=(Type value)
{
	*(Type*)getAddress() &= value;
	return *(Type*)getAddress();
}
}

#endif