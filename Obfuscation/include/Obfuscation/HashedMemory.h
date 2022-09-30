#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>

namespace Obfuscation {

    extern int randomInt();

    enum class WriteDetectionMode {
        None,
        ExceptionOnWrite,
        AccessViolationOnWrite
    };

    class HashedMemory {
    public:
        HashedMemory(WriteDetectionMode wdm);

        __forceinline void write(unsigned char *data, int size) {
            if (m_memorySize != size * 2 + 1) {
                m_memorySize = size * 2 + 1;
                m_dataMemory = std::shared_ptr<unsigned char[]>(new unsigned char[m_memorySize]);
            }
            m_initialized = true;
            m_hashSeeds.clear();
            m_lastMapHash = 0;
            for (int dataIdx = 0; dataIdx < floor(m_memorySize / 2); dataIdx++) {
                auto obfuscationByte = static_cast<unsigned char>(randomInt());
                memcpy_s(&m_dataMemory[dataIdx * 2], 1, &data[dataIdx], 1);
                m_hashSeeds.push_back(randomInt());
                m_dataMemory[dataIdx * 2] = ~m_dataMemory[dataIdx * 2] ^ m_hashSeeds.back();
                m_dataMemory[dataIdx * 2 + 1] = obfuscationByte;
                m_lastMapHash += m_hashGenerator(obfuscationByte);
                m_lastMapHash += m_hashGenerator(m_hashSeeds.back());
                m_lastMapHash += m_hashGenerator(data[dataIdx]);
            }

        }

        __forceinline std::shared_ptr<unsigned char[]> read() {
            if (!m_initialized)
                throw std::runtime_error("Tried to read uninitialized memory");
            size_t fullMapHash = 0;
            std::shared_ptr<unsigned char[]> result(new unsigned char[floor(m_memorySize / 2)]);
            for (int dataIdx = 0; dataIdx < floor(m_memorySize / 2); dataIdx++) {
                memcpy_s(&result[dataIdx], 1, &m_dataMemory[dataIdx * 2], 1);
                result[dataIdx] = ~result[dataIdx] ^ m_hashSeeds[dataIdx];
                fullMapHash += m_hashGenerator(m_dataMemory[dataIdx * 2 + 1]);
                fullMapHash += m_hashGenerator(m_hashSeeds[dataIdx]);
                fullMapHash += m_hashGenerator(result[dataIdx]);
            }
            if (fullMapHash != m_lastMapHash) {
                writeDetected();
            }
            return result;
        }

    private:
        void writeDetected();

        int m_memorySize{0};
        std::shared_ptr<unsigned char[]> m_dataMemory;
        size_t m_lastMapHash{0};
        std::vector<int> m_hashSeeds;
        static std::hash<int> m_hashGenerator;
        WriteDetectionMode m_writeDetectionMode;
        bool m_initialized{false};
    };
}