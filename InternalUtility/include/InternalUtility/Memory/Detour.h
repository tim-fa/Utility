#pragma once

#ifndef DETOUR_H
#define DETOUR_H

#include <Windows.h>

namespace Hook
{
extern void* detour(unsigned char* src, const unsigned char* dst, int len);
extern void* Tramp64(void* src, void* dst, int len);
}

#endif
