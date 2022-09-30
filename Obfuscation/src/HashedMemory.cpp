#include <random>
#include "Obfuscation/HashedMemory.h"


namespace Obfuscation {

    int randomInt() {
        static std::random_device dev;
        static std::mt19937 rng(dev());
        static std::uniform_int_distribution<std::mt19937::result_type> dist(1, 2 * INT_MAX);
        return dist(rng);
    }


    HashedMemory::HashedMemory(WriteDetectionMode wdm)
            : m_writeDetectionMode(wdm){
    }

    void HashedMemory::writeDetected() {
        if (m_writeDetectionMode == WriteDetectionMode::AccessViolationOnWrite) {
            *(int *) nullptr = 1337;
        } else if (m_writeDetectionMode == WriteDetectionMode::ExceptionOnWrite) {
            throw std::runtime_error("Illegal write operation on data!");
        }
    }
}