#pragma once

#include "HashedMemory.h"

namespace Obfuscation {



    template<typename Type>
    class EncryptedVariable {
    public:
        explicit EncryptedVariable();

        __forceinline void set(Type value);

        __forceinline Type get();

        __forceinline operator Type();

        __forceinline EncryptedVariable<Type> &operator=(const Type &value);

        __forceinline EncryptedVariable<Type> operator+(const Type &value);

        __forceinline EncryptedVariable<Type> operator-(const Type &value);

        __forceinline EncryptedVariable<Type> operator++(int);

        __forceinline EncryptedVariable<Type> operator--(int);

    private:
        HashedMemory memory;
    };
}
