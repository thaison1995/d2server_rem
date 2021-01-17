#ifndef INCLUDED_GAME_OFFSETS_H
#define INCLUDED_GAME_OFFSETS_H

#include <windows.h>

DWORD GetDllOffset(int num);
DWORD GetDllOffset(const char* dll, int offset);

#endif