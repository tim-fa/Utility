#pragma once

#include "Obfuscator.h"

namespace Obfuscation {

    template<typename Type>
    Obfuscator<Type>::operator Type() {
        return get();
    }

    template<typename Type>
    Obfuscator<Type>::Obfuscator()
            : memory(sizeof(Type), WriteDetectionMode::ExceptionOnWrite) {
    }

    template<typename Type>
    Obfuscator<Type> Obfuscator<Type>::operator--(Type) {
        auto result = get() - 1;
        set(result);
        return *this;
    }

    template<typename Type>
    Obfuscator<Type> Obfuscator<Type>::operator++(Type) {
        auto result = get() + 1;
        set(result);
        return *this;
    }

    template<typename Type>
    Type Obfuscator<Type>::get() {
        return *(Type*)memory.read().get();
    }

    template<typename Type>
    Obfuscator<Type> Obfuscator<Type>::operator-(const Type &value) {
        auto result = get() - value;
        return Obfuscator<Type>(result);
    }

    template<typename Type>
    Obfuscator<Type> &Obfuscator<Type>::operator=(const Type &value) {
        set(value);
        return *this;
    }

    template<typename Type>
    Obfuscator<Type> Obfuscator<Type>::operator+(const Type &value) {
        auto result = get() + value;
        return Obfuscator<Type>(result);
    }

    template<typename Type>
    void Obfuscator<Type>::set(Type value) {
        memory.write(reinterpret_cast<unsigned char*>(&value));
    }

}