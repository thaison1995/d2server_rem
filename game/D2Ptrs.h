#undef DrawText
#undef DrawTextEx

#ifndef __DEFINE_EXPTRS

#define EXFUNCPTR(d1,v1,t1,t2,t3, o1)  t1 (t2 *d1##_##v1)t3;
#define EXASMPTR(d1,v1,o1)   DWORD d1##_##v1;
#define EXVARPTR(d1,v1,t1,o1)  t1 *d1##_##v1;

#else
enum { DDLL_D2CLIENT, DDLL_D2COMMON, DDLL_D2GFX, DDLL_D2LANG, DDLL_D2WIN, DDLL_D2NET, DDLL_D2GAME, DDLL_D2LAUNCH, DDLL_FOG, DDLL_BNCLIENT, DDLL_STORM, DDLL_D2CMP, DDLL_D2MULTI, DDLL_D2SOUND, DDLL_D2GDI, DDLL_D2DDRAW, DDLL_D2DIRECT3D, DDLL_D2GLIDE };

#define DLLOFFSET(a1,b1) ((DDLL_##a1)|((b1)<<8))

#define EXFUNCPTR(d1,v1,t1,t2,t3, o1) typedef t1 t2 d1##_##v1##_t t3; D2Funcs.##d1##_##v1 = (d1##_##v1##_t *)GetDllOffset(DLLOFFSET(d1,o1));
#define EXASMPTR(d1,v1,o1)   D2Ptrs.##d1##_##v1 = GetDllOffset(DLLOFFSET(d1,o1));
#define EXVARPTR(d1,v1,t1,o1)   D2Vars.##d1##_##v1 = (t1*)GetDllOffset(DLLOFFSET(d1,o1));
#endif

#ifndef __DEFINE_EXPTRS
struct _d2f {
#else
_d2f D2Funcs = { 0 }; void SetupD2Funcs() {
#endif


		//D2COMMON
		//Skill Funcs
		EXFUNCPTR(D2COMMON, GetUnitStat, int, __stdcall, (UnitAny * ptUnit, int statId, int16_t stat2), -10973) //k
		EXFUNCPTR(D2COMMON, GetSkillById, Skill*, __fastcall, (UnitAny * ptUnit, int SkillId, int SkillFlags), -10984) //k
		EXFUNCPTR(D2COMMON, GetSkillType, int, __stdcall, (UnitAny * ptUnit, Skill * ptSkill), -10274) //k
		EXFUNCPTR(D2COMMON, GetSkillId, int, __stdcall, (Skill * ptSkill, char* szFile, int aLine), -11151) //k

		EXFUNCPTR(D2COMMON, GetSkillDmgBonus, int, __stdcall, (UnitAny * ptUnit, int FormulaNo, int SkillId, int SkillLvl), -10586) // k
		EXFUNCPTR(D2COMMON, GetPhysMinDmg, int, __stdcall, (UnitAny * ptUnit, int SkillId, int SkillLvl, int aZero), -10687) //k
		EXFUNCPTR(D2COMMON, GetPhysMaxDmg, int, __stdcall, (UnitAny * ptUnit, int SkillId, int SkillLvl, int aZero), -10196) //k
		EXFUNCPTR(D2COMMON, GetEleDmgDuration, int, __stdcall, (UnitAny * ptUnit, int SkillId, int SkillLvl, int aOne), -11154) //k
		EXFUNCPTR(D2COMMON, GetEleMinDmg, int, __stdcall, (UnitAny * ptUnit, int SkillId, int SkillLvl, int aOne), -10728) //k
		EXFUNCPTR(D2COMMON, GetEleMaxDmg, int, __stdcall, (UnitAny * ptUnit, int SkillId, int SkillLvl, int aOne), -10662) //k
		EXFUNCPTR(D2COMMON, EvaluateSkill, int, __stdcall, (UnitAny * ptUnit, int FormulaNo, int SkillId, int SkillLvl), -11081) //k

		EXFUNCPTR(D2COMMON, GetLeftSkill, Skill*, __stdcall, (UnitAny * ptUnit), -10909) //k
		EXFUNCPTR(D2COMMON, GetRightSkill, Skill*, __stdcall, (UnitAny * ptUnit), -10942) //k

		//Map Funcs
		EXFUNCPTR(D2COMMON, GetLevelNoByRoom, int, __stdcall, (Room1 * ptRoom), -10691) // k
		EXFUNCPTR(D2COMMON, GetActNoByLevelNo, int, __stdcall, (int dwLevelNo), -10864) //k
		EXFUNCPTR(D2COMMON, GetTownLevel, int, __stdcall, (int dwActNo), -10353) //k
		EXFUNCPTR(D2COMMON, AddRoomData, void, __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, Room1 * pRoom), -10890) //k
		EXFUNCPTR(D2COMMON, RemoveRoomData, void, __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, Room1 * pRoom), -10208) //k
		EXFUNCPTR(D2COMMON, GetLevel, Level*, __fastcall, (ActMisc * pMisc, int dwLevelNo), -10283) //k
		EXFUNCPTR(D2COMMON, GetLayer, AutoMapLayer2*, __fastcall, (DWORD dwLevelNo), -10087) //k
		EXFUNCPTR(D2COMMON, GetObjectTxt, ObjectTxt*, __stdcall, (DWORD objno), -10319) //k
		EXFUNCPTR(D2COMMON, GetRoomXYByLevel, Room1*, __stdcall, (Act * ptAct, int LevelNo, int Unk0, int* xPos, int* yPos, int UnitAlign), -10632) //k // Player Align =  2
		EXFUNCPTR(D2COMMON, GetRoomByXY, Room1*, __stdcall, (Act * ptAct, int nX, int nY), -11056) // k
		EXFUNCPTR(D2COMMON, CheckXYType, BOOL, __stdcall, (UnitAny * pUnit, int pX, int pY, int Type), -10841)
		EXFUNCPTR(D2COMMON, MapToAbsScreen, void, __stdcall, (long* pX, long* pY), -10582) //k
		EXFUNCPTR(D2COMMON, AbsScreenToMap, void, __stdcall, (long* mX, long* mY), -10720) //k
		EXFUNCPTR(D2COMMON, GetUnitXOffset, int, __stdcall, (UnitAny * pUnit), -10641) //k
		EXFUNCPTR(D2COMMON, GetUnitYOffset, int, __stdcall, (UnitAny * pUnit), -10057) //k

		//Stat funcs
		EXFUNCPTR(D2COMMON, SetStat, void, __stdcall, (UnitAny * ptUnit, int nStat, int nValue, int nLayer), -10590) // 1.13d
		EXFUNCPTR(D2COMMON, GetStatSigned, int, __stdcall, (UnitAny * ptUnit, int nStat, int nLayer), -10550) // 1.13d
		EXFUNCPTR(D2COMMON, GetStatListStatNum, int, __stdcall, (StatListEx * pStatList, int nStat, Stat * *pOutStats, int nOutStats), -10086) // returns number of found stat in statlist
		EXFUNCPTR(D2COMMON, GetBaseStatSigned, int, __stdcall, (UnitAny * ptUnit, int nStat, int nLayer), -10216) // 1.13d
		EXFUNCPTR(D2COMMON, GetUnitState, int, __stdcall, (UnitAny * ptUnit, DWORD dwStateNo), -10706) //k
		EXFUNCPTR(D2COMMON, GetUnitMaxLife, unsigned int, __stdcall, (UnitAny * ptUnit), -10574) //k
		EXFUNCPTR(D2COMMON, GetUnitMaxMana, unsigned int, __stdcall, (UnitAny * ptUnit), -10084) //k
		EXFUNCPTR(D2COMMON, GetUnitRoom, Room1*, __stdcall, (UnitAny * ptUnit), -10846) //k
		EXFUNCPTR(D2COMMON, GetExpToAchiveLvl, int, __stdcall, (int ClassId, int ExpLvl), -10949) //k
		EXFUNCPTR(D2COMMON, GetMercCost, int, __stdcall, (UnitAny * pItem), -10243)
		EXFUNCPTR(D2COMMON, GetSkillLevel, int, __stdcall, (UnitAny * pUnit, Skill * pSkill, int bTotal), -10306)
		EXFUNCPTR(D2COMMON, StatListsMerge, void, __stdcall, (UnitAny * ptUnit, UnitAny *pItem, int a3), -10164)
		EXFUNCPTR(D2COMMON, ExpireUnitStatList, void, __stdcall, (UnitAny * pUnit, UnitAny* pItem), -11127)

		//Item Related
		EXFUNCPTR(D2COMMON, GetItemText, ItemsTxt*, __stdcall, (int RecordNo), -10994) //k
		EXFUNCPTR(D2COMMON, GetItemIdx, ItemsTxt*, __stdcall, (DWORD ItemCode, int* Idx), -10000)
		EXFUNCPTR(D2COMMON, GetRunesTxt, RunesTxt*, __stdcall, (int RecordNo), -10405)
		EXFUNCPTR(D2COMMON, GetRunesTxtRecords, int*, __stdcall, (void), -10981)
		EXFUNCPTR(D2COMMON, GetAffixTxt, AutoMagicTxt*, __stdcall, (int affix), -10174) // Valid for ItemData->Suffix and Prefix

		EXFUNCPTR(D2COMMON, GetItemByBodyLoc, UnitAny*, __stdcall, (Inventory * pInventory, int aLoc), -10292) //k
		EXFUNCPTR(D2COMMON, GetItemFlag, BOOL, __stdcall, (UnitAny * item, DWORD flagmask, DWORD lineno, char* filename), -10458) //k
		EXFUNCPTR(D2COMMON, GetItemColor, BYTE*, __stdcall, (UnitAny * ptPlayer, UnitAny * ptItem, BYTE * out, BOOL a4), -11062) //k
		EXFUNCPTR(D2COMMON, GetItemCost, int, __stdcall, (UnitAny * pPlayer, UnitAny * ptItem, int DiffLvl, QuestFlags * pQuestFlags, int NpcClassId, int InvPage), -10186)
		EXFUNCPTR(D2COMMON, GetItemType, int, __stdcall, (UnitAny * pItem), -10121)
		EXFUNCPTR(D2COMMON, IsMatchingType, BOOL, __stdcall, (UnitAny * pItem, int iType), -10601)

		//D2NET
		EXFUNCPTR(D2NET, SendPacket, bool, __stdcall, (int PacketLen, int _1, BYTE * aPacket), -10015) // k
		EXFUNCPTR(D2NET, ReceivePacket, void, __fastcall, (int* eLen, BYTE * aPacket, int aLen), 0x7450) //k

		//D2CMP
		EXFUNCPTR(D2CMP, DeleteCellFile, void, __stdcall, (CellFile * cf), -10020) //k
		EXFUNCPTR(D2CMP, MixPalette, BYTE*, __stdcall, (int TransLvl, int ColorNo), -10009) //k
		EXFUNCPTR(D2CMP, SetupGfxCell, BOOL, __stdcall, (CellContext * cc, int aZero, int aOne), -10025)
		EXFUNCPTR(D2CMP, SetupGfxTile, BOOL, __stdcall, (TileContext * tc, int aZero, int aOne), -10084)
		EXFUNCPTR(D2CMP, DrawCellOnBitmapBuffer, void, __stdcall, (CellContext * pContext, int nX, int nY, int a4, int nWidth, void* pBitmapBuffer, int BitmapWidth, int BitmapHeight, int a9, int a10, BYTE * bPalette), -10001)
		//FOG
		EXFUNCPTR(FOG, AllocMemory, void*, __fastcall, (int MemSize, char* szFile, int Line, int aNull), -10042)
		EXFUNCPTR(FOG, FreeMemory, void, __fastcall, (void* MemPool, char* szFile, int Line, int aNull), -10043)
		EXFUNCPTR(FOG, AllocServerMemory, void*, __fastcall, (void* pMemPool, int MemSize, char* szFile, int Line, int aNull), -10045)
		EXFUNCPTR(FOG, FreeServerMemory, void, __fastcall, (void* pMemPool, void* Memory, char* szFile, int Line, int aNull), -10046)
		EXFUNCPTR(FOG, Error, void, __cdecl, (const char* File, void* Addr, int Line), -10024)
		EXFUNCPTR(FOG, GetErrorData, void*, __cdecl, (), -10265)
		EXFUNCPTR(FOG, isExpansion, BOOL, __cdecl, (), -10227)
		EXFUNCPTR(FOG, isDirect, BOOL, __cdecl, (), -10117)
		EXFUNCPTR(FOG, InitBitBuffer, void, __stdcall, (BitBuffer * pBitBuffer, void* Source, int Size), -10126)
		EXFUNCPTR(FOG, ReadBits, unsigned int, __stdcall, (BitBuffer * pBuffer, int nBits), -10130)
		EXFUNCPTR(FOG, ReadBitsSigned, signed int, __stdcall, (BitBuffer * pBuffer, int nBits), -10129)
		EXFUNCPTR(FOG, DecompressPacket, unsigned int, __fastcall, (BYTE * dest, DWORD memory_size, BYTE * source, DWORD src_size), -10224)
		EXFUNCPTR(FOG, GetBinTxtRowByText, int, __stdcall, (void* pLink, const char* szText, DWORD nColumn), -10217)
		EXFUNCPTR(FOG, GetBinTxtIndex, int, __stdcall, (void* pLink, DWORD dwOrigin, DWORD _1), -10213)

		//STORM
		EXFUNCPTR(STORM, CloseArchive, BOOL, __stdcall, (HANDLE hArchive), -252)

		EXFUNCPTR(STORM, RegisterCommand, BOOL, __stdcall, (HANDLE hWnd, WPARAM wParam, void(__stdcall * fnCallBack)(StormMsg*)), -414)
		EXFUNCPTR(STORM, RegisterKeyDown, BOOL, __stdcall, (HANDLE hWnd, DWORD vKey, void(__stdcall * fnCallBack)(StormMsg*)), -415)
		EXFUNCPTR(STORM, RegisterKeyUp, BOOL, __stdcall, (HANDLE hWnd, DWORD vKey, void(__stdcall * fnCallBack)(StormMsg*)), -416)
		EXFUNCPTR(STORM, RegisterMsg, BOOL, __stdcall, (HANDLE hWnd, WPARAM wParam, void(__stdcall * fnCallBack)(StormMsg*)), -417)

		EXFUNCPTR(STORM, UnregisterCommand, BOOL, __stdcall, (HANDLE hWnd, WPARAM wParam, void(__stdcall * fnCallBack)(StormMsg*)), -512)
		EXFUNCPTR(STORM, UnregisterKeyDown, BOOL, __stdcall, (HANDLE hWnd, DWORD vKey, void(__stdcall * fnCallBack)(StormMsg*)), -513)
		EXFUNCPTR(STORM, UnregisterKeyUp, BOOL, __stdcall, (HANDLE hWnd, DWORD vKey, void(__stdcall * fnCallBack)(StormMsg*)), -514)
		EXFUNCPTR(STORM, UnregisterMsg, BOOL, __stdcall, (HANDLE hWnd, WPARAM wParam, void(__stdcall * fnCallBack)(StormMsg*)), -515)

		EXFUNCPTR(STORM, ResetMsgQuene, BOOL, __stdcall, (StormMsg * psMsg), -511)


		//D2LANG
			/*
		EXFUNCPTR(D2LANG, UnloadStrings, void, __fastcall, (void), -10000)
		EXFUNCPTR(D2LANG, GetLocaleId, int, __fastcall, (void), -10001) //k //ns
		EXFUNCPTR(D2LANG, GetLocaleText, wchar_t*, __fastcall, (short nLocaleTxtNo), -10004) //k
		EXFUNCPTR(D2LANG, GetLocaleFolder, void, __stdcall, (char* out, int aZero), -10008)
		EXFUNCPTR(D2LANG, LoadStrings, BOOL, __fastcall, (int _1, const char* szLang, BOOL bExpansion), -10009) // _1 seems unused
		*/

		EXFUNCPTR(D2GAME, Unsummon, int, __fastcall, (int a3, UnitAny* pUnit, int v11, int a4), 0x97B50)

		// D2GS
		EXFUNCPTR(FOG, InitLogManager, void, __fastcall, (const char* szPath), -10021)
		EXFUNCPTR(FOG, InitSystem, int, __fastcall, (const char* szProgramName, void* fpErrorHandle, const char* szBuild, int unk), -10019)
		EXFUNCPTR(FOG, SetWorkingDirectory, int, __fastcall, (int grfDefaultSearchScope, int bEnableSeekOptimization), -10101)
		EXFUNCPTR(FOG, MPQFileOpen, int, __fastcall, (const char* szFileName, void** pHandle), -10102)
		EXFUNCPTR(FOG, InitAsyncData, int, __fastcall, (int, int), -10089)
		EXFUNCPTR(FOG, InitMaskTables, int, __fastcall, (), -10218)
		EXFUNCPTR(D2WIN, LoadMPQs, int, __stdcall, (), -10086)
		EXFUNCPTR(D2LANG, PreInitLang, void, __fastcall, (), 0xA6C8)
		EXFUNCPTR(D2LANG, InitLanguage, int, __fastcall, (int, const char*, int), -10008)
		EXFUNCPTR(D2COMMON, LoadDataTables, int, __stdcall, (void*, int, int), -10943)
		EXFUNCPTR(FOG, SetQServerParams, int, __stdcall, (int*), -10185)
		EXFUNCPTR(D2NET, StartQServer, int, __stdcall, (DWORD, DWORD), -10005)
		EXFUNCPTR(D2NET, AddClientToHackList, int, __stdcall, (int, char*, int), -10001)

		EXFUNCPTR(D2GAME, HandleClientPackets, int, __stdcall, (), -10040)
		EXFUNCPTR(D2GAME, UpdateServerFrame, int, __stdcall, (DWORD), -10008)
		EXFUNCPTR(D2GAME, DispatchPacketsToClients, int, __stdcall, (DWORD, DWORD), -10024)
		EXFUNCPTR(D2GAME, EndAllGames, int, __stdcall, (), -10006)
		EXFUNCPTR(D2GAME, SetupCallbackTable, int, __stdcall, (void*), -10048)
		EXFUNCPTR(D2GAME, SetupCallback01, int, __stdcall, (void*, int*), -10037)
		EXFUNCPTR(D2GAME, SetupCallback02, int, __stdcall, (), -10038)
		EXFUNCPTR(D2GAME, CreateGame, int, __stdcall, (const char* lpGameName, const char* lpGamePass,
			const char* lpGameDesc, DWORD dwGameFlag,
			BYTE bTemplate, BYTE bReserved1, BYTE bReserved2, LPWORD pwGameId), -10044)
		EXFUNCPTR(D2GAME, SendDatabaseCharacter, int, __stdcall, (DWORD dwClientId, LPVOID lpSaveData,
			DWORD dwSize, DWORD dwTotalSize, BOOL bLock, DWORD dwReserved1, void* lpPlayerInfo, 
			DWORD dwReserved2), -10035)

		EXFUNCPTR(STORM, SFileSetPlatform, void, __stdcall, (int dwPlatform), -278)
		EXFUNCPTR(STORM, SFileSetLocale, void, __stdcall, (WORD langId), -272)

#ifndef __DEFINE_EXPTRS
};
extern _d2f D2Funcs;
struct _d2p {
#else
}
_d2p D2Ptrs = { 0 };
void SetupD2Pointers() {
#endif

		EXASMPTR(D2GAME, GetGameByClientID_I, 0x2AB70)
		EXASMPTR(D2GAME, LeaveCriticalSection_I, 0x29BA0)
		EXASMPTR(D2GAME, SendPacket_I, 0x8A3E0)
		EXASMPTR(D2GAME, ParseIncomingPacket_I, 0xCC550)
		EXASMPTR(D2GAME, ParseCreatePacket_I, 0xCAF30)

		EXASMPTR(D2NET, ReceivePacket_I, -10001) // k [isPacketCorrect]
		EXASMPTR(D2GAME, FindFreeCoords_I, 0xE0000)
		EXASMPTR(D2GAME, CheckXYOccupy_I, 0x1340)
		EXASMPTR(D2GAME, SaveCharToDatabase_I, 0x54320)
		EXASMPTR(D2GAME, SavePlayerData_I, 0x2C910)

#ifndef __DEFINE_EXPTRS
};
extern _d2p D2Ptrs;
struct _d2v {
#else
}
_d2v D2Vars = { 0 };
void SetupD2Vars() {
#endif

		//EXVARPTR(D2CLIENT, sgptDataTables, sgptDataTable*, 0xCF474) // k
		EXVARPTR(D2COMMON, sgptDataTables, sgptDataTable*, -11170) // eon
		EXVARPTR(D2COMMON, InventoryTxt, InventoryTxt*, 0xA4CAC)
		EXVARPTR(D2COMMON, AutoMagicTxt, AutoMagicTxt*, 0xA4CE4)

		//-----Packets things
		EXVARPTR(D2NET, SrvPacketLenTable, int, 0xABD8) //k <- Same as in 1.11b


#ifndef __DEFINE_EXPTRS
};
extern _d2v D2Vars;
#else
}
#endif

#undef DLLOFFSET
#undef __DEFINE_EXPTRS
#undef EXFUNCPTR
#undef EXVARPTR
#undef EXASMPTR

