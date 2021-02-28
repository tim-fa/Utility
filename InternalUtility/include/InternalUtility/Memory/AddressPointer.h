#pragma once

#ifndef ADDRESS_PTR_H
#define ADDRESS_PTR_H

#include <vector>
#include "InternalUtility.h"

namespace Internals
{
template<typename Type = long>
class AddressPointer
{
	public:
		AddressPointer() = default;
		template<typename... Args>
		explicit AddressPointer(Args... offsets);
		template<typename... Args>
		void initialize(Args... offsets);
		void printPointerPath();
		operator Type&() const;
		AddressPointer<Type>& operator=(Type value);
		long operator&();
		Type* operator->();

		long getAddress();
		void setValue(Type value);

	private:
		void initializePointer();
		void addOffset(long offset);
		long currentAddress{0};
		std::vector<int> offsets;
};

template<typename Type>
template<typename... Args>
inline AddressPointer<Type>::AddressPointer(Args... offsets)
{
	initialize(offsets...);
}

template<typename Type>
inline void AddressPointer<Type>::printPointerPath()
{
	long curAdr = Internals::getModuleBase();
	int i = 0;
	for (auto& offset : offsets) {
		long pointedValue = *(long*)(curAdr + offset);
		printf("Level %2d: 0x%010X + 0x%05X [0x%010X] -> 0x%X (Decimal: %d)\n", i++, curAdr, offset, curAdr + offset, pointedValue,
			pointedValue);
		curAdr = pointedValue;
	}
}

template<typename Type>
template<typename... Args>
void AddressPointer<Type>::initialize(Args... offsets)
{
	for (long offset : {offsets...}) {
		addOffset(offset);
	}
}

template<typename Type>
inline void AddressPointer<Type>::addOffset(long offset)
{
	offsets.push_back(offset);
}

template<typename Type>
inline AddressPointer<Type>::operator Type&() const
{
	return *(Type*)getAddress();
}

template<typename Type>
inline AddressPointer<Type>& AddressPointer<Type>::operator=(Type value)
{
	setValue(value);
	return *this;
}

template<typename Type>
inline long AddressPointer<Type>::operator&()
{
	return getAddress();
}

template<typename Type>
inline Type* AddressPointer<Type>::operator->()
{
	return (Type*)getAddress();
}

template<typename Type>
long AddressPointer<Type>::getAddress()
{
	initializePointer();
	return currentAddress;
}

template<typename Type>
void AddressPointer<Type>::setValue(Type value)
{
	initializePointer();
	*(Type*)currentAddress = value;
}

template<typename Type>
void AddressPointer<Type>::initializePointer()
{
	__try{
		currentAddress = Internals::getModuleBase();
		bool first = true;
		for (auto& offset : offsets) {
			if (!first) {
				currentAddress = *(long*)currentAddress;
			}
			currentAddress = currentAddress + offset;
			first = false;
		}
		long test = *(long*)currentAddress;
		
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		currentAddress = 0;
		throw std::runtime_error("Access violation! Address is not (yet) accessible");
	}
}

}

#endif