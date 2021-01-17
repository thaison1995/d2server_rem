#ifndef INCLUDED_GAME_MISC_H
#define INCLUDED_GAME_MISC_H

#include <windows.h>

#define PATCH_CALL 0xE8
#define PATCH_JUMP 0xE9
#define PATCH_NOP 0x90
#define PATCH_RET 0xC3
#define PATCH_XOR 0x33
#define PATCH_CUSTOM 0

void Patch(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen, const char* Type);

#endif