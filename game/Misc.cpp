#include <windows.h>
#include "Misc.h"
#include <exception>

BOOL WriteBytes(LPVOID lpAddr, LPVOID lpBuffer, DWORD dwLen)
{
	DWORD dwOldProtect;
	if (!VirtualProtect(lpAddr, dwLen, PAGE_READWRITE, &dwOldProtect))
		return FALSE;

	memcpy(lpAddr, lpBuffer, dwLen);

	if (!VirtualProtect(lpAddr, dwLen, dwOldProtect, &dwOldProtect))
		return FALSE;

	return TRUE;
}

BOOL WriteDword(DWORD* lpAddr, DWORD lpBuffer)
{
	DWORD dwOldProtect;
	if (!VirtualProtect(lpAddr, 4, PAGE_READWRITE, &dwOldProtect))
	{
		throw std::exception("Failed to patch");
	}
	*lpAddr = lpBuffer;

	if (!VirtualProtect(lpAddr, 4, dwOldProtect, &dwOldProtect))
	{
		throw std::exception("Failed to patch");
	}
	return TRUE;
}

void Patch(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen, const char* Type)
{
	BYTE* bCode = new BYTE[dwLen];
	if (bInst)
	{
		::memset(bCode, 0x90, dwLen);
		bCode[0] = bInst;
		if (pFunc)
		{
			if (bInst == 0xE8 || bInst == 0xE9)
			{
				DWORD dwFunc = pFunc - (pAddr + 5);
				*(DWORD*)&bCode[1] = dwFunc;
			}
			else
				if (bInst == 0x68 || bInst == 0x05 || bInst == 0x5B)
				{
					*(LPDWORD)&bCode[1] = pFunc;
				}
				else
					if (bInst == 0x83)
					{
						*(WORD*)&bCode[1] = (WORD)pFunc;
					}
					else
					{
						bCode[1] = (BYTE)pFunc;
					}
		}
	}
	else
	{
		if (dwLen == 4)
			*(DWORD*)&bCode[0] = pFunc;
		else if (dwLen == 2)
			*(WORD*)&bCode[0] = (WORD)pFunc;
		else if (dwLen == 1)
			*(BYTE*)&bCode[0] = (BYTE)pFunc;
		else {
			memcpy(bCode, (void*)pFunc, dwLen);
		}
	}

	if (!WriteBytes((void*)pAddr, bCode, dwLen))
	{
		delete[] bCode;
		throw std::exception("Error while patching game");
	}
	delete[] bCode;
}
