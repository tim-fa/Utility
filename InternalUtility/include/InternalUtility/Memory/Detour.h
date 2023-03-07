#pragma once

#ifndef DETOUR_H
#define DETOUR_H

#include <Windows.h>
#include <string>
#include <memory>
#include <utility>
#include <vector>

namespace Hooking {
    extern void *detour(unsigned char *src, const unsigned char *dst, int len);

    class DetourHook {
        struct RelativeInstruction {
            RelativeInstruction(const std::string &instructionName, const std::vector<byte> &bytes)
                    : name(instructionName), bytes(bytes) {
            }

            std::string name;
            std::vector<byte> bytes;
        };

    public:

        DetourHook();

        void *trampoline64(void *src, void *dst, int len);

    private:

        int relativeJumpPresent(void *address);

        void addRelativeInstruction(const std::string &instructionName, const std::vector<byte> &bytes);

        std::vector<RelativeInstruction> m_relativeInstructions;
    };
}

#endif
