#pragma once

#ifndef FUNCTION_PTR_H
#define FUNCTION_PTR_H

#include <vector>
#include "InternalUtility.h"
#include "AddressPointer.h"

template<typename ReturnType, typename... Arguments>
class FunctionPointer {
public:
	FunctionPointer(long offset)
		: address(offset)
	{
	}

	ReturnType operator()(Arguments... arguments)
	{
		ReturnType(*func)(Arguments...) = ((ReturnType(*)(Arguments...))&address);
		return func(arguments...);
	}

private:
	AddressPointer<long> address;
};

#endif