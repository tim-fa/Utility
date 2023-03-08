#pragma once

#ifndef DETOUR_H
#define DETOUR_H

#include <Windows.h>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <InternalUtility/Memory/InstructionHandler.h>


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
        void *trampoline64(byte *src, void *dst, int len);

    private:

        RelativeInstructionHandler m_instructionHandler;

    };
}

#endif
