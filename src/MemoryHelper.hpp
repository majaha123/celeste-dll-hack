#pragma once
#ifndef MEMORY_HELPER
#define MEMORY_HELPER

#include <Windows.h>
#include <vector>

class MemoryHelper
{
public:
    static DWORD GetAddressFromSignature(const std::vector<int> &signature, DWORD startaddress = 0, DWORD endaddress = 0);
};

#endif