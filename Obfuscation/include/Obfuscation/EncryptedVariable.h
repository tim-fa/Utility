#pragma once

#include "HashedMemory.h"

namespace Obfuscation {

    template<typename Type>
    class EncryptedVariable {
    public:
        explicit EncryptedVariable();

        __forceinline void set(Type value, int size);

        __forceinline Type get();

        __forceinline operator Type();

        __forceinline EncryptedVariable<Type> &operator=(const Type &value);

        __forceinline EncryptedVariable<Type> operator+(const Type &value);

        __forceinline EncryptedVariable<Type> operator-(const Type &value);

        __forceinline EncryptedVariable<Type> operator++(int);

        __forceinline EncryptedVariable<Type> operator--(int);

    protected:
        HashedMemory memory;
    };

    template<typename Type>
    class EncryptedVariable<Type*> : public EncryptedVariable<__int64> {
    public:
        __forceinline EncryptedVariable<Type*> &operator=(Type* value)
        {
            this->set((__int64)value, sizeof(__int64));
            return *this;
        }

        __forceinline Type* operator->()
        {
            return (Type*)this->get();
        }
    };
}
