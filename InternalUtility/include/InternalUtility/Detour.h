#pragma once

#ifndef DETOUR_H
#define DETOUR_H

#include <Windows.h>

namespace Internals
{
extern void* detour(unsigned char* src, const unsigned char* dst, int len);
}

#endif
