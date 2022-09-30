#pragma once

#include "EncryptedVariable.h"

namespace Obfuscation {

    template<typename Type>
    EncryptedVariable<Type>::operator Type() {
        return get();
    }

    template<typename Type>
    EncryptedVariable<Type>::EncryptedVariable()
            : memory(sizeof(Type), WriteDetectionMode::ExceptionOnWrite) {
    }

    template<typename Type>
    EncryptedVariable<Type> EncryptedVariable<Type>::operator--(int) {
        auto result = get() - 1;
        set(result);
        return *this;
    }

    template<typename Type>
    EncryptedVariable<Type> EncryptedVariable<Type>::operator++(int) {
        auto result = get() + 1;
        set(result);
        return *this;
    }

    template<typename Type>
    Type EncryptedVariable<Type>::get() {
        return *(Type*)memory.read().get();
    }

    template<typename Type>
    EncryptedVariable<Type> EncryptedVariable<Type>::operator-(const Type &value) {
        auto result = get() - value;
        return Obfuscator<Type>(result);
    }

    template<typename Type>
    EncryptedVariable<Type> &EncryptedVariable<Type>::operator=(const Type &value) {
        set(value);
        return *this;
    }

    template<typename Type>
    EncryptedVariable<Type> EncryptedVariable<Type>::operator+(const Type &value) {
        auto result = get() + value;
        return Obfuscator<Type>(result);
    }

    template<typename Type>
    void EncryptedVariable<Type>::set(Type value) {
        memory.write(reinterpret_cast<unsigned char*>(&value));
    }

}