#include "server/logger.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "d2server.h"

#include "game/Offsets.h"
#include "game/D2Constants.h"
#include "game/D2Structs.h"
#include "game/D2Ptrs.h"
#define __DEFINE_EXPTRS
#include "game/D2Ptrs.h"
#undef __DEFINE_EXPTRS

#include "game/Misc.h"

#include <windows.h>
#undef ERROR

#include <string>

#pragma comment(lib, "server/d2server.lib")

#include "game/MPQHelper.h"

#include <utility>

#include "game/HelperFunction.hpp"
#include "net/d2cs_d2gs_character.h"

#include "turbo/engine.h"

namespace Server {
	int __stdcall ErrorHandler(int addr, int a2, int a3, char* fmt...) {
		va_list args;
		va_start(args, fmt);
		const char* s = va_arg(args, char*);
		va_end(args);
		LOG(INFO) << "Error at 0x" << std::hex << addr << ": " << s;
		return 0;
	}

	int __stdcall ErrorHandler2(char* a, int x, char* fmt...) {
		LOG(INFO) << "Error";
		return 0;
	}

	void __cdecl GSLog(LPCSTR lpModule, LPCSTR lpFormat, ...) {
		va_list     ap;

		va_start(ap, lpFormat);
		char buf[4096];
		vsprintf_s(buf, lpFormat, ap);
		LOG(INFO) << lpModule << ": " << buf;
		va_end(ap);
	}

	void InitD2Env() {
		SetupD2Vars();
		SetupD2Pointers();
		SetupD2Funcs();
	}

	static void* dword_func00;

	void D2Server::OnNextFrame(OnFrameEvent f)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		on_frame_events_.push_back(f);
	}

	void D2Server::PatchD2() {
		LOG(INFO) << "Patching FOG Exception Handler";
		Patch(PATCH_CALL, GetDllOffset("Fog.dll", 0x18BA0), (DWORD)ErrorHandler, 5, "");
		Patch(PATCH_CALL, GetDllOffset("Fog.dll", 0x10A22), (DWORD)ErrorHandler2, 5, "");

		int fogBase = GetDllOffset("Fog.dll", 0);

		// replace Fog MemoryPool with ours
		DWORD Fog_D2MemoryPool_pManagers = 0x6FF9AF94;

		int patched = 0;
		for (int i = 0; i < 0x23BC7; i++) {
			int* p = (int*)(i + fogBase);
			if (*p == Fog_D2MemoryPool_pManagers) {
				DWORD fpOld;
				VirtualProtect(p, 4, PAGE_READWRITE, &fpOld);
				*p = (int)game_pool_managers_;
				VirtualProtect(p, 4, fpOld, &fpOld);
				LOG(INFO) << "Patched FOG+0x" << std::hex << i;
				patched++;
			}
		}
		LOG(INFO) << "Patched " << patched << " addresses";

		// from D2server.dll patch table
		const char patchData[] = { 0x59, 0x58, 0x51, 0x33, 0xC0, 0xC3 };
		Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0xDD10), (DWORD)patchData, 6, "");

		Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0x1AEF9), (WORD)0x31EB, 2, "");

		Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0x1AEDF), Fog_D2MemoryPool_pManagers, 4, "");
		Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0x1B98A), Fog_D2MemoryPool_pManagers, 4, "");
		Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0x10F78), (DWORD)0, 4, "");

		// avoid setting 6FFA6E74 to 1
		Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0x118FD), (DWORD)0x90909090, 4, "");
		Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0x11901), (BYTE)0x90, 1, "");

		// 1.13 CompareFileTime workaround
		Patch(PATCH_CUSTOM, GetDllOffset("D2Game.dll", 0xEBB2A), (WORD)0xEB, 1, "");

		// from patch table 
		Patch(PATCH_CUSTOM, GetDllOffset("D2Game.dll", 0x52743), (WORD)0xEB09, 2, "");

		dword_func00 = (void*)(*(DWORD*)GetDllOffset("D2Client.dll", 0xCDC21));

		LOG(INFO) << "All D2 patches applied successfully";
	}

	int D2Server::ServerInit() {
		// D2GSStart
		//   run MyPatchInit 1.asm
		//     D2GSPreInit
		//   MyPatchInit2
		//     D2GSInit
		//     ret to 6800106Dh

		// d2 seq: https://d2mods.info/forum/viewtopic.php?t=62807

		// fog memory pool: https://d2mods.info/forum/viewtopic.php?f=8&t=61294&p=476474#p476474
		LOG(INFO) << "Initializing Storm";

		// because d2server.lib is statically linked and it imports storm.dll
		// this somehow triggered some magic that helps storm.dll to initialize its mpq crypt table
		// if we don't do this, we will end up with corrupted mpq archives in memory
		LPD2GSINTERFACE		lpD2GSInterface;
		lpD2GSInterface = ::QueryInterface();

		InitD2Env();

		LOG(INFO) << "Creating D2 game memory pools";
		DWORD memSize = sizeof(D2PoolManagerStrc) * 102;
		game_pool_managers_ = (D2PoolManagerStrc*)VirtualAlloc(0, memSize, MEM_COMMIT, PAGE_READWRITE);
		if (!game_pool_managers_) {
			LOG(ERROR) << "Failed to create memory pools";
			return -1;
		}
		memset(game_pool_managers_, 0, memSize);

		PatchD2();

		LOG(INFO) << "Initializing FOG Log Manager";

		const char* logPrefix = "GS-";
		D2Funcs.FOG_InitLogManager(logPrefix);

		LOG(INFO) << "Initializing FOG System";
		D2Funcs.FOG_InitSystem("Diablo II Game Server", NULL, "D2GSLib 1.10.3.4 Build On Feb 29 2004 22:31:15", 1);

		// Storm ordinal func list: https://github.com/nickshanks/Alkor/blob/master/Source/MPQReader.m
		D2Funcs.FOG_SetWorkingDirectory(0, 0);

		LOG(INFO) << "Initializing FOG data";
		D2Funcs.FOG_InitAsyncData(1, 0);
		D2Funcs.FOG_InitMaskTables();

		LOG(INFO) << "Initializing MPQs";
		if (!D2Funcs.D2WIN_LoadMPQs()) {
			LOG(ERROR) << "Failed to initialize MPQs";
			return -1;
		}

		void* mpqHandle = 0;
		int vret2 = FindInMPQ("DATA\\LOCAL\\FONT\\LATIN\\DEFAULT.MAP");
		int vret = D2Funcs.FOG_MPQFileOpen("DATA\\LOCAL\\FONT\\LATIN\\DEFAULT.MAP", &mpqHandle);

		if (!vret) {
			LOG(ERROR) << "Something went wrong with MPQs: errno " << GetLastError();
			return -1;
		}

		LOG(INFO) << "Initializing Language";
		if (!D2Funcs.D2LANG_InitLanguage(0, "ENG", 1)) {
			LOG(ERROR) << "Failed to initialize language";
			return -1;
		}

		LOG(INFO) << "Initializing Data Tables";
		D2Funcs.D2COMMON_LoadDataTables(NULL, 1, 1);

		LOG(INFO) << "Initializing QServer";
		int v[4] = { 100, 200, 300, 400 };
		int ret1 = D2Funcs.FOG_SetQServerParams((int*)v);
		int ret2 = D2Funcs.D2NET_StartQServer(0, 0);

		LOG(INFO) << "Initializing Callback Table";
		D2Funcs.D2GAME_SetupCallbackTable(&callback_table_);

		int v7 = 0;
		D2Funcs.D2GAME_SetupCallback01(dword_func00, &v7);
		D2Funcs.D2GAME_SetupCallback02();

		LOG(INFO) << "D2GSInit finished";
		return 0;
	}

	void D2Server::CallbackGetDatabaseCharacter(std::string acctname, std::string charname, int client_id) {
		PlayerRef curr_player = player(charname);
		if (curr_player == nullptr) {
			LOG(ERROR) << "Char " << charname << " does not exist in players_";
			return;
		}
		curr_player->client_id = client_id;

		net_manager_->d2dbs_client().GetCharsaveDataAsync(acctname, charname,
			[acctname, charname, client_id, &curr_player](bool allow_ladder, int char_create_time, std::string charsave) {
			curr_player->locked = true;
			PLAYERINFO pi;
			pi.PlayerMark = 0;
			pi.dwReserved = 0;
			strcpy_s((char*)&pi.AcctName, 16, curr_player->acctname.c_str());
			strcpy_s((char*)&pi.CharName, 16, curr_player->charname.c_str());
			if (!D2Funcs.D2GAME_SendDatabaseCharacter(client_id, (void*)charsave.c_str(),
				charsave.length(), charsave.length(), false, 0, &pi, 0x1)) {
				LOG(ERROR) << "Failed to call D2GAME_SendDatabaseCharacter";
			}
			else {
				LOG(INFO) << "Successfully loaded character " << charname << " (create_time=" << char_create_time
					<< ",ladder=" << allow_ladder << ",size=" << charsave.length() << ")";
			}
			}, [client_id](Net::D2DBSClient::GetDataFailureReason reason) {
				LOG(ERROR) << "Can't get charsave for client " << client_id;
			}
		);
	}

	D2Server::PlayerRef D2Server::CallbackFindPlayerToken(std::string charname, join_request_token token, int game_id)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		if (pending_join_requests_.find(token) == pending_join_requests_.end()) {
			return nullptr;
		}
		auto& req = pending_join_requests_[token];
		if (req.charname != charname || req.game_id != game_id) {
			return nullptr;
		}
		pending_join_requests_.erase(token);
		return players_[charname];
	}

	void D2Server::CallbackCloseGame(int game_id)
	{
		GameRef curr_game = game(game_id);
		if (curr_game == nullptr) {
			LOG(ERROR) << "Game " << game_id << " not found";
			return;
		}

		net_manager_->d2dbs_client().GameSignalAsync(curr_game->qualified_game_name(), true);
		net_manager_->d2cs_client().CloseGameAsync(game_id);
		games_.erase(game_id);
		LOG(INFO) << "Game has been closed: " << curr_game->game_name << " (id=" << game_id << ")";
	}

	void D2Server::Player::UpdateD2CS(bool enter, bool leave)
	{
		PROTO_UPDATEGAMEINFO_FLAG flag = PROTO_UPDATEGAMEINFO_FLAG::D2GS_D2CS_UPDATEGAMEINFO_FLAG_UPDATE;
		if (enter) {
			flag = PROTO_UPDATEGAMEINFO_FLAG::D2GS_D2CS_UPDATEGAMEINFO_FLAG_ENTER;
		}
		if (leave) {
			flag = PROTO_UPDATEGAMEINFO_FLAG::D2GS_D2CS_UPDATEGAMEINFO_FLAG_LEAVE;
		}

		parent_.net_manager_->d2cs_client().UpdateGameInfoAsync(flag, game_id, char_level, char_class, 
			inet_addr(client_ipaddr.c_str()), charname);
	}


	class GameDataGuard {
	public:
		GameDataGuard(const GameDataGuard&) = delete;
		GameDataGuard(int client_id) {
			game_ = Helper_D2GAME_GetGameByClientID(client_id);
			client_data_ = Helper_FindClientDataById(game_, client_id);
		}
		~GameDataGuard() {
			Helper_D2GAME_LeaveCriticalSection(game_);
		}
		Game* game() {
			return game_;
		}
		ClientData* client_data() {
			return client_data_;
		}
	private:
		Game* game_;
		ClientData* client_data_;
	};

	void D2Server::Player::SendChatMessage(std::string sender, std::string msg, int color)
	{
		parent_.OnNextFrame([this, sender, msg, color]() {
			GameDataGuard g(client_id);
			Helper_SendChatMessagePacket(g.client_data(), sender, msg, color);
		});
	}

	void D2Server::Player::Kick()
	{
		parent_.OnNextFrame([this]() {
			GameDataGuard g(client_id);
			Helper_SendKick(g.client_data());
		});
	}

	void D2Server::Player::Save(std::string& charsave)
	{
		parent_.net_manager_->d2dbs_client().SaveCharsaveAsync(acctname, charname, client_ipaddr, charsave);
	}

	void D2Server::Player::SaveCharinfo(std::string& charinfo)
	{
		parent_.net_manager_->d2dbs_client().SaveCharinfoAsync(acctname, charname, charinfo);
	}

	void D2Server::Player::LeaveGame()
	{
		if (locked) {
			parent_.net_manager_->d2dbs_client().CharLockAsync(acctname, charname, false);
		}
		if (in_game) {
			UpdateD2CS(false, true);
		}
		// remove *this from players_ map
		parent_.players_.erase(charname);
		LOG(INFO) << "Player " << charname << " removed from map";
	}

	void D2Server::Player::UpdateLadderInformation(int exp_low, int exp_high, int char_status)
	{
		parent_.net_manager_->d2dbs_client().UpdateLadderAsync(acctname, charname, char_class, char_level, 
			exp_low, exp_high, char_status);
	}

	void D2Server::ServerLoop() {
		while (!net_manager_->d2cs_client().authed()) {
			LOG(WARNING) << "Waiting for auth from D2CS...";
			Sleep(1000);
		}

		LOG(INFO) << "Setting up game server handlers";
		net_manager_->d2cs_client().OnCreateGame([this](t_d2cs_d2gs_creategamereq& req, int& game_id) -> bool {
			int game_flag = 0x04;
			if (req.expansion) game_flag |= 0x100000;
			if (req.hardcore)  game_flag |= 0x800;
			if (req.ladder)    game_flag |= 0x200000;
			if (req.difficulty <= 2) game_flag |= ((req.difficulty) << 0x0c);
			unsigned short out_game_id = 0;
			int result = D2Funcs.D2GAME_CreateGame(req.gamename.c_str(), req.gamepass.c_str(), req.gamedesc.c_str(), 
				game_flag, 0x11, 0x22, 0x33, &out_game_id);
			if (result) {
				game_id = out_game_id;

				std::shared_ptr<GameInfo> game_info = std::make_shared<GameInfo>(
					*this, game_id, req.gamename);
				net_manager_->d2dbs_client().GameSignalAsync(game_info->qualified_game_name(), false);
				games_.emplace(game_id, std::move(game_info));

				LOG(INFO) << "Created game " << req.gamename << "(id=" << out_game_id << ") with flag " << game_flag
					<< " (expansion=" << (int)req.expansion << ",hardcore=" << (int)req.hardcore
					<< ",ladder=" << (int)req.ladder << ",difficulty=" << (int)req.difficulty << ")";
				return true;
			}
			else {
				LOG(ERROR) << "Failed to create the game " << req.gamename << " with flag " << game_flag
					<< " (expansion=" << (int)req.expansion << ",hardcore=" << (int)req.hardcore
					<< ",ladder=" << (int)req.ladder << ",difficulty=" << (int)req.difficulty << ")";
				return false;
			}
		});
		net_manager_->d2cs_client().OnJoinGame([this](t_d2cs_d2gs_joingamereq& req) -> bool {
			JoinRequest r;
			r.token = req.token;
			r.game_id = req.gameid;
			r.charname = req.charname;
			r.acctname = req.acctname;
			r.client_ipaddr = req.client_ipaddr;
			{
				std::lock_guard<std::mutex> guard(mutex_);
				pending_join_requests_[req.token] = r;

				std::shared_ptr<Player> player = std::make_shared<Player>(*this, req.charname, req.acctname, r.client_ipaddr, r.game_id);
				players_.emplace(req.charname, std::move(player));
			}
			LOG(INFO) << "Added player " << req.charname << "(*" << req.acctname << ") to join game "
				<< req.gameid << " (client address is " << req.client_ipaddr << ")";
			return true;
		});
		net_manager_->d2cs_client().SetGSInfoAsync(100, 0);

		LOG(INFO) << "Entering D2 server loop";
		while (1) {
			while (on_frame_events_.size() > 0) {
				auto f = on_frame_events_.front();
				on_frame_events_.pop_front();
				f();
			}
			D2Funcs.D2GAME_HandleClientPackets();
			if (D2Funcs.D2GAME_UpdateServerFrame(0)) {
				// busy
				D2Funcs.D2GAME_DispatchPacketsToClients(0, 0);
				Sleep(40);
			}
			else {
				// idle
				Sleep(40);
			}
		}
		D2Funcs.D2GAME_EndAllGames();
		D2Funcs.D2GAME_DispatchPacketsToClients(1, 0);
	}

	static D2Server* g_server;

	extern void __fastcall GetDatabaseCharacter(LPGAMEDATA lpGameData, LPCSTR lpCharName,
		DWORD dwClientId, LPCSTR lpAccountName)
	{
		g_server->CallbackGetDatabaseCharacter(lpAccountName, lpCharName, dwClientId);
	}

	extern BOOL __fastcall FindPlayerToken(LPCSTR lpCharName, DWORD dwToken, WORD wGameId,
		LPSTR lpAccountName, LPPLAYERDATA lpPlayerData, int a1, int a2, int a3, int a4)
	{
		D2Server::PlayerRef player = g_server->CallbackFindPlayerToken(lpCharName, dwToken, wGameId);
		if (player == nullptr) {
			return false;
		}
		if (player->game_id == wGameId) {
			strcpy_s(lpAccountName, 16, player->acctname.c_str());
			*lpPlayerData = (PLAYERDATA)0x01;
			return true;
		}
		return false;
	}

	extern void __fastcall EnterGame(WORD wGameId, LPCSTR lpCharName, WORD wCharClass,
		DWORD dwCharLevel, DWORD dwReserved)
	{
		D2Server::PlayerRef player = g_server->player(lpCharName);
		if (player == nullptr) {
			LOG(ERROR) << "Invalid player " << lpCharName << " entering game";
		}
		if (player->game_id != wGameId) {
			LOG(ERROR) << "GameId for player " << lpCharName << " mismatch";
		}
		player->char_class = wCharClass;
		player->char_level = dwCharLevel;
		player->in_game = true;
		player->enter_game_time = std::time(nullptr);
		player->UpdateD2CS(true, false);

		LOG(INFO) << "Player " << player->charname << "(*" << player->acctname << ") added to game " << wGameId;

		// TODO: use motd from conf
		player->SendChatMessage("", "Welcome to D2Server REmastered", D2COLOR_ID_RED);
	}

	extern void __fastcall SaveDatabaseCharacter(LPGAMEDATA lpGameData, LPCSTR lpCharName,
		LPCSTR lpAccountName, LPVOID lpSaveData,
		DWORD dwSize, PLAYERDATA PlayerData)
	{
		D2Server::PlayerRef player = g_server->player(lpCharName);
		if (player == nullptr) {
			LOG(ERROR) << "Invalid player " << lpCharName << " during SaveDatabaseCharacter";
		}
		std::string charsave((const char*)lpSaveData+sizeof(short), dwSize-sizeof(short));
		player->Save(charsave);
	}

	extern void __fastcall LeaveGame(LPGAMEDATA lpGameData, WORD wGameId, WORD wCharClass,
		DWORD dwCharLevel, DWORD dwExpLow, DWORD dwExpHigh,
		WORD wCharStatus, LPCSTR lpCharName, LPCSTR lpCharPortrait,
		BOOL bUnlock, DWORD dwZero1, DWORD dwZero2,
		LPCSTR lpAccountName, PLAYERDATA PlayerData,
		DWORD dwZero3, DWORD dwZero4, DWORD dwZero5, DWORD dwZero6, DWORD dwZero7, int saveTimestamp)
	{
		D2Server::PlayerRef player = g_server->player(lpCharName);
		if (player == nullptr) {
			LOG(ERROR) << "Invalid player " << lpCharName << " during LeaveGame";
		}
		player->char_class = wCharClass;
		player->char_level = dwCharLevel;

		std::string portrait(lpCharPortrait);
		std::string charinfo = assemble_charinfo(player->acctname, player->charname, player->char_class, 
			player->char_level, dwExpLow, wCharStatus, portrait, player->enter_game_time, g_server->realm_name());
		player->SaveCharinfo(charinfo);
		player->LeaveGame();
	}

	extern void __fastcall CloseGame(WORD wGameId, DWORD dwGameVersion, int nUnitCounter, int nGamePlayTime)
	{
		g_server->CallbackCloseGame(wGameId);
	}

	extern void __fastcall UpdateCharacterLadder(LPCSTR lpCharName, WORD wCharClass,
		DWORD dwCharLevel, DWORD dwCharExpLow,
		DWORD dwCharExpHigh, WORD wCharStatus, PLAYERMARK PlayerMark)
	{
		D2Server::PlayerRef player = g_server->player(lpCharName);
		if (player == nullptr) {
			LOG(ERROR) << "Invalid player " << lpCharName << " during UpdateCharacterLadder";
		}
		player->char_class = wCharClass;
		player->char_level = dwCharLevel;
		player->UpdateLadderInformation(dwCharExpLow, dwCharExpHigh, wCharStatus);
	}

	extern void __fastcall UpdateGameInformation(WORD wGameId, LPCSTR lpCharName,
		WORD wCharClass, DWORD dwCharLevel)
	{
		D2Server::PlayerRef player = g_server->player(lpCharName);
		if (player == nullptr) {
			LOG(ERROR) << "Invalid player " << lpCharName << " during UpdateGameInformation";
		}
		player->char_class = wCharClass;
		player->char_level = dwCharLevel;
		player->UpdateD2CS(false, false);
	}

	#include "LegacyGSCallback.hpp"

	D2Server::D2Server(bool legacy, Net::NetManagerRef net_manager)
		:legacy_(legacy), net_manager_(net_manager)
	{
		g_server = this;

		SetupCallbackTable(callback_table_);

		gs_info_.szVersion = "GSRem";
		gs_info_.dwLibVersion = 0x010A0304;
		gs_info_.bIsNT = 1;
		gs_info_.bEnablePatch = 1;
		gs_info_.fpEventLog = (EventLogFunc)GSLog;
		gs_info_.fpErrorHandle = (FARPROC)D2GSErrorHandle;
		gs_info_.fpCallback = &callback_table_;
		gs_info_.bPreCache = 1;
		gs_info_.dwIdleSleep = 40;
		gs_info_.dwBusySleep = 40;
		gs_info_.dwMaxGame = 16;
		gs_info_.dwReserved0 = 1200;
		gs_info_.bStop = FALSE;
		HANDLE			hObjects[2];
		hObjects[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
		gs_info_.hEventInited = hObjects[0];
		memset(gs_info_.dwReserved, 0, sizeof(DWORD) * 24);
	}

	void D2Server::Run()
	{
		if (legacy_) {
			::D2GSStartFunc gsStart;
			LPD2GSINTERFACE		lpD2GSInterface;

			lpD2GSInterface = ::QueryInterface();

			gsStart = lpD2GSInterface->D2GSStart;

			InitD2Env();

			// avoid setting 6FFA6E74 to 1
			Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0x118FD), (DWORD)0x90909090, 4, "");
			Patch(PATCH_CUSTOM, GetDllOffset("Fog.dll", 0x11901), (BYTE)0x90, 1, "");
			DWORD addr = (DWORD)D2Funcs.D2GAME_SetupCallback01;

			DWORD			dwThreadId;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)gsStart, &gs_info_, 0, &dwThreadId);

			Sleep(500);

			Sleep(500000);
		}
		else {
			if (ServerInit() != 0) {
				throw std::exception("Failed to initialize D2Server");
			}
			ServerLoop();
		}
	}


}