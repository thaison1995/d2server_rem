#include "MPQHelper.h"

static int cryptTable[0x500];

void prepareCryptTable() {
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;
	for (index1 = 0; index1 < 0x100; index1++)
	{
		for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{
			unsigned long temp1, temp2;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;

			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);

			cryptTable[index2] = (temp1 | temp2);
		}
	}
}

unsigned long HashString(const char* lpszFileName, unsigned long dwHashType)
{
	unsigned char* key = (unsigned char*)lpszFileName;
	unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch;

	while (*key != 0)
	{
		ch = toupper(*key++);

		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

#include <cassert>

int FindInMPQ(const char* fileName) {
	MPQ_Handle* first = (MPQ_Handle*)*((DWORD*)0x6FC45984);
	prepareCryptTable();
	while ((int)first > 0) {
		const char* mpqData = first->mpqData;
		MPQ_HashEntry* hashTable = first->hashTable;
		int hashSize = *((int*)(mpqData + 0x18));

		const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
		DWORD nHash = HashString(fileName, HASH_OFFSET);
		DWORD nHashA = HashString(fileName, HASH_A);
		DWORD nHashB = HashString(fileName, HASH_B);
		int nHashStart = nHash % hashSize;
		int nHashPos = nHashStart;

		if (strcmp(first->mpqName, "d2data.mpq") == 0) {
			//LOG(INFO) << "Finding " << fileName << " in " << first->mpqName << ": hashStart is 0x" << std::hex << nHashStart;

			while (hashTable[nHashPos].dwBlockIndex != 0xFFFFFFFF)
			{
				if (hashTable[nHashPos].hash2 == nHashA && hashTable[nHashPos].hash3 == nHashB)
					return nHashPos;
				else
					nHashPos = (nHashPos + 1) % hashSize;

				if (nHashPos == nHashStart)
					break;
			}
		}


		first = first->next;
	}

	return -1;
}
