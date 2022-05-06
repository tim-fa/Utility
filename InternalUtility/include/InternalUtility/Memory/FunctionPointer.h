#pragma once

#ifndef FUNCTION_PTR_H
#define FUNCTION_PTR_H

#include <vector>
#include "InternalUtility.h"
#include "Variable.h"

namespace Memory {
    template<typename ReturnType, typename... Arguments>
    class FunctionPointer {
    public:
        FunctionPointer() = default;

        explicit FunctionPointer(long offset)
                : address(offset) {
        }

        void initialize(long offset) {
            address.initialize(offset);
        }

        ReturnType operator()(Arguments... arguments) {
            ReturnType (*func)(Arguments...) = ((ReturnType(*)(Arguments...)) &address);
            return func(arguments...);
        }

        Variable<long> getAddressPointer() {
            return address;
        }

    private:
        Variable<long> address;
    };
}

#endif