#ifndef INCLUDED_GAME_D2MEM_POOL_H
#define INCLUDED_GAME_D2MEM_POOL_H

#include <Windows.h>

#define MAX_POOL_NAME 32
#define MAX_POOLS 40
#define MAX_MANAGERS 8
#define MAX_POOL_OVERFLOW 1023

struct D2PoolStrc;

struct D2PoolBlockStrc				//sizeof 0x18
{
	BYTE* pCommit;					//+00
	DWORD* pUsage;					//+04
	size_t nBlocks;					//+08
	D2PoolBlockStrc* pPrev;			//+0C
	D2PoolBlockStrc* pNext;			//+10
	D2PoolStrc* pPool;				//+14
};

struct D2PoolStrc				//sizeof 0x30
{
	CRITICAL_SECTION pSync;		//+00
	size_t nBlockSize;			//+18
	size_t nBlocks;				//+1C
	size_t nSize;				//+20
	size_t nAllocBlock;			//+24
	D2PoolBlockStrc* pBlocks;	//+28
	D2PoolBlockStrc* pTail;		//+2C	
};

struct D2PoolBlockEntryStrc		//sizeof 0x8
{
	D2PoolBlockStrc* pBlock;	//+00
	void* pCommit;				//+04
};

struct D2PoolManagerStrc					//sizeof 0x17CC	
{
	DWORD dw;								//+00 - related to pool count..
	CRITICAL_SECTION pSync;					//+04
	size_t nPools;							//+1C
	D2PoolStrc pPools[MAX_POOLS];			//+20
	size_t nBlocks;							//+7A0
	size_t nTotalBlocks;					//+7A4
	D2PoolBlockEntryStrc* pBlocks;			//+7A8
	BYTE* pOverflow[MAX_POOL_OVERFLOW];		//+7AC
	DWORD dwMemory;							//+17A8
	char szName[MAX_POOL_NAME];				//+17AC	
};

struct D2MemoryManagerStrc						//sizeof 0xBEA4
{
	BOOL bInit;									//+00
	CRITICAL_SECTION pSync;						//+04
	D2PoolManagerStrc pManagers[MAX_MANAGERS];	//+1C
	size_t nManagers;							//+BE7C
	size_t nManagerIndex[8];					//+BE80
	size_t nFreeManagers;						//+BEA0
};


#endif