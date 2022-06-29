#pragma once

#ifndef FUNCTION_PTR_H
#define FUNCTION_PTR_H

#include <vector>
#include "InternalUtility.h"
#include "Typedefs.h"
#include "Variable.h"

namespace Hooking {
    template<typename ReturnType, typename... Arguments>
    class FunctionPointer {
    public:
        FunctionPointer() = default;

        explicit FunctionPointer(long offset)
                : address(offset) {
        }

        void initialize(long offset) {
            address.initializeByOffsets(offset);
        }

        ReturnType operator()(Arguments... arguments) {
            ReturnType (*func)(Arguments...) = ((ReturnType(*)(Arguments...)) &address);
            return func(arguments...);
        }

        Variable<BaseType_t> getAddressPointer() {
            return address;
        }

    private:
        Variable<BaseType_t> address;
    };
}

#endif