#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>

#include "d2server.h"

#include "game/Offsets.h"
#include "game/D2Constants.h"
#include "game/D2Structs.h"
#include "game/D2Ptrs.h"
#define __DEFINE_EXPTRS
#include "game/D2Ptrs.h"

#include "game/Misc.h"

#include <windows.h>
#undef ERROR

#include <string>

#pragma comment(lib, "server/d2server.lib")

#include "game/MPQHelper.h"

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
		Patch(PATCH_CUSTOM, GetDllOffset("D2Game.dll", 0xEBB55), (WORD)0x9090, 2, "");

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

	void D2Server::ServerLoop() {
		LOG(WARNING) << "Waiting for auth from D2CS";
		while (!net_manager_->d2cs_client().authed()) {
			Sleep(1000);
		}

		LOG(WARNING) << "Setting up game server handlers";
		net_manager_->d2cs_client().OnCreateGame([this](t_d2cs_d2gs_creategamereq& req, int& game_id) -> bool {
			int game_flag = 0x04;
			if (req.expansion) game_flag |= 0x100000;
			if (req.hardcore)  game_flag |= 0x800;
			if (req.ladder)    game_flag |= 0x200000;
			if (req.difficulty <= 2) game_flag |= ((req.difficulty) << 0x0c);
			unsigned short out_game_id = 0;
			LOG(INFO) << "Creating game " << req.gamename << " with flag " << game_flag
				<< " (expansion=" << (int)req.expansion << ",hardcore=" << (int)req.hardcore 
				<< ",ladder=" << (int)req.ladder<< ",difficulty=" << (int)req.difficulty << ")";
			int result = D2Funcs.D2GAME_CreateGame(req.gamename.c_str(), req.gamepass.c_str(), req.gamedesc.c_str(), 
				game_flag, 0x11, 0x22, 0x33, &out_game_id);
			if (result) {
				LOG(INFO) << "Created successfully. Game id is " << out_game_id;
				game_id = out_game_id;
				return true;
			}
			else {
				LOG(ERROR) << "Failed to create the game";
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
			pending_join_requests_[req.token] = r;
			LOG(INFO) << "Added request for " << req.charname << "(*" << req.acctname << ") to join game "
				<< req.gameid << " (client address is " << req.client_ipaddr << ")";
			return true;
		});
		net_manager_->d2cs_client().SetGSInfoAsync(100, 0);

		LOG(INFO) << "Entering D2 server loop";
		while (1) {
			D2Funcs.D2GAME_10040();
			if (D2Funcs.D2GAME_10008(0)) {
				// busy
				D2Funcs.D2GAME_10024(0, 0);
				Sleep(40);
			}
			else {
				// idle
				Sleep(40);
			}
		}
		D2Funcs.D2GAME_EndAllGames();
		D2Funcs.D2GAME_10024(1, 0);
	}

	#include "LegacyGSCallback.hpp"

	D2Server::D2Server(bool legacy, Net::NetManagerRef net_manager)
		:legacy_(legacy), net_manager_(net_manager)
	{
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