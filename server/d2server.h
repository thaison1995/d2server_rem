#ifndef INCLUDED_D2SERVER_H
#define INCLUDED_D2SERVER_H

#include "net/net_manager.h"
#include "game/D2MemPool.h"
#include "d2server_lib.h"

#include <unordered_map>

namespace Server {
	using join_request_token = int;
	struct JoinRequest {
		int token;
		int game_id;
		std::string charname;
		std::string acctname;
		std::string client_ipaddr;
	};

	class D2Server {
	public:
		D2Server(const D2Server&) = delete;
		D2Server(bool legacy, Net::NetManagerRef net_manager);
		void Run();

	private:
		bool legacy_;
		D2GSINFO gs_info_;
		EVENTCALLBACKTABLE callback_table_;
		D2PoolManagerStrc* game_pool_managers_;
		Net::NetManagerRef net_manager_;
		std::unordered_map<join_request_token, JoinRequest> pending_join_requests_;

		void PatchD2();
		int ServerInit();
		void ServerLoop();
	};
}

#endif