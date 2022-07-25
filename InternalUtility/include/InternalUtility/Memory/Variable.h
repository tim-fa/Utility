#pragma once

#include <vector>
#include <stdexcept>
#include <exception>
#include "InternalUtility.h"
#include "Typedefs.h"
#include "PatternScanner.h"
#include "Log/Logger.h"

#undef __try

#define DebugPrintPointerPath(var) Log::Logger::Debug("{}:", #var); var.printPointerPath()

namespace Hooking {
    template<typename DataType>
    class Variable {
    public:
        Variable() = default;

        template<typename... Args>
        explicit Variable(Args... offsets)
                : offs({offsets...}) {
        }

        template<typename... Args>
        void initializeByOffsets(Args... offsets) {
            offs.clear();
            for (auto &offset: {offsets...}) {
                offs.push_back(offset);
            }
        }

        void initializeByPattern(const std::string &pattern) {
            initializeByOffsets(findPlaceholderAddress(pattern) - getModuleBase());
        }

        void printPointerPath() {
            BaseType_t curAdr = getModuleBase();
            int i = 0;
            for (auto &offset: offs) {
                BaseType_t pointedValue = *(BaseType_t *) (curAdr + offset);
                Log::Logger::d("Level {:2d}: 0x{:010X} + 0x{:06X} [0x{:010X}] -> 0x{:X} (Decimal: {:d})\n", i++, curAdr,
                               offset,
                               curAdr + offset, pointedValue, pointedValue);
                curAdr = pointedValue;
            }
        }

        operator DataType &() {
            return *(DataType *) getAddress();
        }

        Variable<DataType> &operator=(DataType value) {
            setValue(value);
            return *this;
        }

        DataType operator&=(DataType value) {
            *(DataType *) getAddress() &= value;
            return *(DataType *) getAddress();
        }

        DataType *operator&() {
            return (DataType *) getAddress();
        }

        BaseType_t getAddress() {
            initializePointer();
            return currentAddress;
        }

        void setValue(DataType value) {
            *(DataType *) getAddress() = value;
        }

    private:
        void initializePointer() {
            __try
            {
                currentAddress = getModuleBase();
                bool first = true;
                for (auto &offset: offs) {
                    if (!first) {
                        currentAddress = *(BaseType_t *) currentAddress;
                    }
                    currentAddress = currentAddress + offset;
                    first = false;
                }
                volatile int value = *(BaseType_t *) currentAddress;
            }
            __except(EXCEPTION_EXECUTE_HANDLER)
            {
                throw std::runtime_error("Variable::initializePointer(): Access violation");
            }
        }

        void addOffset(long offset) {
            offs.push_back(offset);
        }

        BaseType_t currentAddress{0};
        std::vector<int> offs;
    };

    template<typename DataType>
    class Variable<DataType *> : public Variable<DataType> {
    public:
        DataType *operator->() {
            return (DataType *) Variable<DataType>::getAddress();
        }
    };


// Arrays can be interpreted as pointers
    template<typename DataType>
    class Variable<DataType[]> : public Variable<DataType *> {
    public:
        DataType &operator[](int index) {
            return *(DataType *) (Variable<DataType>::getAddress() + sizeof(DataType) * index);
        }
    };

}
