#pragma once


#include <memory>


static int randomInt() {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<std::mt19937::result_type> dist(1, 2 * INT_MAX);
    return dist(rng);
}


class HashedMemory {
public:
    HashedMemory(int size)
            : m_memorySize(size * 2 + 1), m_dataMemory(new unsigned char[m_memorySize]), m_initialized(false) {
    }

    __forceinline size_t getMemoryHash() {
        size_t fullMapHash = 0;
        for (int dataIdx = 0; dataIdx < floor(m_memorySize / 2) && m_initialized; dataIdx++) {
            static std::hash<int> hg;
            fullMapHash += hg(m_dataMemory[dataIdx * 2]);
            fullMapHash += hg(m_dataMemory[dataIdx * 2 + 1]);
            fullMapHash += hg(m_hashSeeds[dataIdx]);
        }
        return fullMapHash;
    }

    __forceinline void writeMemoryHash()
    {
        m_lastMapHash = 0;
        for (int dataIdx = 0; dataIdx < floor(m_memorySize / 2) && m_initialized; dataIdx++) {
            static std::hash<int> hg;
            m_dataMemory[dataIdx * 2 + 1] = static_cast<unsigned char>(randomInt());
            m_lastMapHash += hg(m_dataMemory[dataIdx * 2]);
            m_lastMapHash += hg(m_dataMemory[dataIdx * 2 + 1]);
            m_lastMapHash += hg(m_hashSeeds[dataIdx]);
        }
    }
    __forceinline void write(unsigned char *data) {
        m_hashSeeds.clear();
        if (getMemoryHash() != m_lastMapHash){
            throw std::runtime_error("Illegal write operation on data!");
        }
        m_initialized = true;
        for (int dataIdx = 0; dataIdx < floor(m_memorySize / 2); dataIdx++) {
            memcpy_s(&m_dataMemory[dataIdx * 2], 1, &data[dataIdx], 1);
            m_hashSeeds.push_back(randomInt());
            m_dataMemory[dataIdx * 2] = ~m_dataMemory[dataIdx * 2] ^ m_hashSeeds.back();
            static std::hash<unsigned char> hg;
            m_dataMemory[dataIdx * 2 + 1] = static_cast<unsigned char>(randomInt());
        }
        writeMemoryHash();
    }

    __forceinline std::shared_ptr<unsigned char[]> read() {
        if (getMemoryHash() != m_lastMapHash){
            throw std::runtime_error("Illegal write operation on data!");
        }
        std::shared_ptr<unsigned char[]> result(new unsigned char[floor(m_memorySize / 2)]);
        for (int dataIdx = 0; dataIdx < floor(m_memorySize / 2); dataIdx++) {
            memcpy_s(&result[dataIdx], 1, &m_dataMemory[dataIdx * 2], 1);
            static std::hash<unsigned char> hg;
            result[dataIdx] = ~result[dataIdx] ^ m_hashSeeds[dataIdx];
        }
        return result;
    }

private:
    const int m_memorySize;
    std::shared_ptr<unsigned char[]> m_dataMemory;
    size_t m_lastMapHash;
    bool m_initialized;
    std::vector<int> m_hashSeeds;
};