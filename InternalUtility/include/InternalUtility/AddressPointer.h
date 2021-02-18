#pragma once

#ifndef ADDRESS_PTR_H
#define ADDRESS_PTR_H

#include <vector>
#include "InternalUtility.h"

namespace Internals
{
template<typename Type>
class AddressPointer
{
	public:
		template<typename... Args>
		AddressPointer(Args... offsets);
		void printPointerPath();
		operator Type&() const;
		Type operator=(const Type value);
		long operator&();
		Type* operator->();

	private:
		void addOffset(long offset);

		std::vector<int> offsets;
		long currentAddress;
};

template<typename Type>
template<typename... Args>
inline AddressPointer<Type>::AddressPointer(Args... offsets)
	: currentAddress(Internals::getModuleBase())
{
	for (long offset : {offsets...}) {
		addOffset(offset);
	}
}

template<typename Type>
inline void AddressPointer<Type>::printPointerPath()
{
	long currentAddress = Internals::getModuleBase();
	int i = 0;
	for (auto& offset : offsets) {
		long pointedValue = *(long*)(currentAddress + offset);
		printf("Level %2d: 0x%010X + 0x%05X [0x%010X] -> 0x%X (Decimal: %d)\n", i++, currentAddress, offset, currentAddress + offset, pointedValue,
			pointedValue);
		currentAddress = pointedValue;
	}
}

template<typename Type>
inline void AddressPointer<Type>::addOffset(long offset)
{
	if (offsets.size() > 0) {
		currentAddress = *(long*)currentAddress;
	}
	offsets.push_back(offset);
	currentAddress = currentAddress + offset;
}

template<typename Type>
inline AddressPointer<Type>::operator Type&() const
{
	return *(Type*)currentAddress;
}

template<typename Type>
inline Type AddressPointer<Type>::operator=(const Type value)
{
	*(Type*)currentAddress = value;
	return value;
}

template<typename Type>
inline long AddressPointer<Type>::operator&()
{
	return currentAddress;
}

template<typename Type>
inline Type* AddressPointer<Type>::operator->()
{
	return (Type*)currentAddress;
}
}

#endif