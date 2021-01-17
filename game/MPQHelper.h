#ifndef INCLUDED_GAME_MPQ_HELPER_H
#define INCLUDED_GAME_MPQ_HELPER_H

#include <Windows.h>

#pragma pack(push, 1)
struct MPQ_HashEntry {
	DWORD hash2;         // 0x00
	DWORD hash3;         // 0x04
	USHORT lcLocale;
	BYTE   Platform;
	BYTE   Reserved;
	DWORD dwBlockIndex;
};
#pragma pack(pop)

struct MPQ_Handle {
	DWORD reserved;          // 0x00
	MPQ_Handle* next;         // 0x04
	char  mpqName[0x20];     // 0x08
	DWORD reserved2[0x42];   // 
	const char* mpqData;     // 0x130
	DWORD reserved3;         // 0x134
	MPQ_HashEntry* hashTable;    // 0x138
};

int FindInMPQ(const char* fileName);

#endif