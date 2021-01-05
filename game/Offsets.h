#pragma once
#include <windows.h>

DWORD GetDllOffset(int num);
DWORD GetDllOffset(const char* dll, int offset);