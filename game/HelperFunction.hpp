#include <windows.h>
#include "D2Structs.h"

static Game* Helper_D2GAME_GetGameByClientID(DWORD dwClientId) {
	Game* lpGame;
	__asm {
		push esi
		mov esi, dwClientId
		call D2Ptrs.D2GAME_GetGameByClientID_I
		pop esi
		mov lpGame, eax
	}
	return lpGame;
}

static ClientData* Helper_FindClientDataById(Game* pGame, DWORD ClientID)
{
	if (!pGame) return 0;

	for (ClientData* pClient = pGame->pClientList; pClient; pClient = pClient->ptPrevious)
	{
		if (pClient->ClientID == ClientID) return pClient;
	}

	return 0;
}

static void __fastcall Helper_D2GAME_LeaveCriticalSection(Game* pGame)
{
	__asm {
		mov eax, ecx  //pGame
		call D2Ptrs.D2GAME_LeaveCriticalSection_I
	}
}

static void __declspec(naked) __fastcall Helper_D2GAME_SendPacket(ClientData* pClientData, BYTE* aPacket, int aLen)
{
	__asm
	{
		mov eax, [esp + 4]
		push eax
		push edx
		mov eax, ecx
		call D2Ptrs.D2GAME_SendPacket_I
		retn 4
	}
}

static void __declspec(naked) __fastcall Helper_D2GAME_SaveCharToDatabase(Game* game, ClientData* clientData, 
	UnitAny* unit, const char* charname) {
	__asm
	{
		push ebp
		mov ebp, esp
		add ebp, 4

		mov eax, [ebp+8]

		push ebx
		mov ebx, ecx
		push edi
		mov edi, ecx
		push esi
		mov esi, [ebp+4]
		mov ebp, edx

		push 1
		push eax
		call D2Ptrs.D2GAME_SaveCharToDatabase_I

		pop esi
		pop edi
		pop ebx
		pop ebp

		retn 8
	}
}

static void Helper_SendChatMessagePacket(ClientData* client, std::string sender, std::string msg, int color) {

	WORD packet_len = sizeof(D2GSPacketSrv26) + sender.length() + 1 + msg.length() + 1;

	char* packet = (char*)malloc(packet_len);
	memset(packet, 0, packet_len);

	D2GSPacketSrv26* s = (D2GSPacketSrv26*)packet;
	s->nHeader = 0x26;
	s->nMessageType = CHAT_MESSAGE_TYPE_SYS_MESSAGE;
	s->nUnitType = 2;
	s->nMessageColor = color;
	strcpy_s(&packet[sizeof(D2GSPacketSrv26)], sender.length() + 1, sender.c_str());
	strcpy_s(&packet[sizeof(D2GSPacketSrv26) + sender.length() + 1], msg.length() + 1, msg.c_str());
	Helper_D2GAME_SendPacket(client, (BYTE*)packet, packet_len);
}

static void Helper_SendKick(ClientData* client) {

	int packet_len = sizeof(D2GSPacketSrvGeneric);
	D2GSPacketSrvGeneric* packet = (D2GSPacketSrvGeneric*)malloc(packet_len);
	memset(packet, 0, packet_len);
	packet->nHeader = 0x06;
	Helper_D2GAME_SendPacket(client, (BYTE*)packet, packet_len);
}

static void __fastcall Helper_SaveAllPlayerDataInGame(ClientData* pClient, Game* pGame) {
	ClientData* curr = pGame->pClientList;
	while (curr) {
		if (curr != pClient && curr->pPlayerUnit) {
			Helper_D2GAME_SaveCharToDatabase(pGame, curr, curr->pPlayerUnit, curr->CharName);
		}
		curr = curr->ptPrevious;
	}
}

static __declspec(naked) void  __fastcall Helper_SavePlayerDataIntercept_STUB() {
	__asm
	{
		push ebp
		mov ebp, esp
		add ebp, 4

		push ecx
		push edx

		mov ecx, [ebp + 4]
		mov edx, [ebp + 8]
		call Helper_SaveAllPlayerDataInGame

		push[ebp + 8]
		push[ebp + 4]
		call D2Ptrs.D2GAME_SavePlayerData_I

		pop edx
		pop ecx
		pop ebp
		retn 8
	}
}