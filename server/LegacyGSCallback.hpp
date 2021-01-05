
extern void __fastcall CloseGame(WORD wGameId)
{
	return;
}


extern void __fastcall LeaveGame(LPGAMEDATA lpGameData, WORD wGameId, WORD wCharClass,
	DWORD dwCharLevel, DWORD dwExpLow, DWORD dwExpHigh,
	WORD wCharStatus, LPCSTR lpCharName, LPCSTR lpCharPortrait,
	BOOL bUnlock, DWORD dwZero1, DWORD dwZero2,
	LPCSTR lpAccountName, PLAYERDATA PlayerData,
	PLAYERMARK PlayerMark)
{
	return;
}


extern void __fastcall GetDatabaseCharacter(LPGAMEDATA lpGameData, LPCSTR lpCharName,
	DWORD dwClientId, LPCSTR lpAccountName)
{
	return;
}


extern void __fastcall SaveDatabaseCharacter(LPGAMEDATA lpGameData, LPCSTR lpCharName,
	LPCSTR lpAccountName, LPVOID lpSaveData,
	DWORD dwSize, PLAYERDATA PlayerData)
{
	return;
}


extern void __fastcall EnterGame(WORD wGameId, LPCSTR lpCharName, WORD wCharClass,
	DWORD dwCharLevel, DWORD dwReserved)

{
	return;
}


extern BOOL __fastcall FindPlayerToken(LPCSTR lpCharName, DWORD dwToken, WORD wGameId,
	LPSTR lpAccountName, LPPLAYERDATA lpPlayerData)
{
	return false;
}


extern void __fastcall UnlockDatabaseCharacter(LPGAMEDATA lpGameData, LPCSTR lpCharName,
	LPCSTR lpAccountName)
{
	return;
}


extern void __fastcall RelockDatabaseCharacter(LPGAMEDATA lpGameData, LPCSTR lpCharName,
	LPCSTR lpAccountName)
{
	return;
}


extern void __fastcall UpdateCharacterLadder(LPCSTR lpCharName, WORD wCharClass,
	DWORD dwCharLevel, DWORD dwCharExpLow,
	DWORD dwCharExpHigh, WORD wCharStatus,
	PLAYERMARK PlayerMark)
{
	return;
}


extern void __fastcall UpdateGameInformation(WORD wGameId, LPCSTR lpCharName,
	WORD wCharClass, DWORD dwCharLevel)
{
	return;
}


extern GAMEDATA __fastcall SetGameData(void)
{
	return (GAMEDATA)0x87654321;
}


extern void __fastcall SaveDatabaseGuild(DWORD dwReserved1, DWORD dwReserved2,
	DWORD dwReserved3)

{
	return;
}


extern void __fastcall ReservedCallback1(DWORD dwReserved1, DWORD dwReserved2)
{
	return;
}


extern void __fastcall ReservedCallback2(DWORD dwReserved1, DWORD dwReserved2,
	DWORD dwReserved3)
{
	return;
}


extern void __fastcall LoadComplete(WORD wGameId, LPCSTR lpCharName, BOOL bExpansion)
{
	return;
}


extern void __cdecl	ServerLogMessage(DWORD dwCount, LPCSTR lpFormat, ...)
{
	va_list     ap;

	va_start(ap, lpFormat);
	LOG(INFO) << "ServerLogMessage: " << lpFormat << ap;
	va_end(ap);
	return;
}

static DWORD __stdcall D2GSErrorHandle(void)
{
	LOG(ERROR) << "D2GSErrorHandle: Error occur, exiting..";

#ifdef DEBUG_ON_CONSOLE
	printf("Press Any Key to Continue");
	_getch();
#endif

	ExitProcess(0);
	return 0;

} /* End of D2GSErrorHandle() */

void SetupCallbackTable(EVENTCALLBACKTABLE& gEventCallbackTable) {
	gEventCallbackTable.fpCloseGame = CloseGame;
	gEventCallbackTable.fpLeaveGame = LeaveGame;
	gEventCallbackTable.fpGetDatabaseCharacter = GetDatabaseCharacter;
	gEventCallbackTable.fpSaveDatabaseCharacter = SaveDatabaseCharacter;
	gEventCallbackTable.fpServerLogMessage = ServerLogMessage;
	gEventCallbackTable.fpEnterGame = EnterGame;
	gEventCallbackTable.fpFindPlayerToken = FindPlayerToken;
	gEventCallbackTable.fpUnlockDatabaseCharacter = UnlockDatabaseCharacter;
	gEventCallbackTable.fpRelockDatabaseCharacter = RelockDatabaseCharacter;
	gEventCallbackTable.fpUpdateCharacterLadder = UpdateCharacterLadder;
	gEventCallbackTable.fpUpdateGameInformation = UpdateGameInformation;
	gEventCallbackTable.fpSetGameData = SetGameData;
	gEventCallbackTable.fpReserved1 = ReservedCallback1;
	gEventCallbackTable.fpReserved2 = ReservedCallback2;
	gEventCallbackTable.fpSaveDatabaseGuild = SaveDatabaseGuild;
	gEventCallbackTable.fpLoadComplete = LoadComplete;
}