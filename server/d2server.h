#ifndef INCLUDED_D2SERVER_H
#define INCLUDED_D2SERVER_H

#include "net/net_manager.h"
#include "game/D2MemPool.h"
#include "d2server_lib.h"

#include <ctime>
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

		void CallbackGetDatabaseCharacter(std::string acctname, std::string charname, int client_id);
		void CallbackCloseGame(int game_id);

		class Player {
		public:
			std::string charname;
			std::string acctname;
			std::string client_ipaddr;
			int game_id;
			int client_id;

			int char_class;
			int char_level;
			bool in_game;
			bool locked;
			std::time_t enter_game_time;

			Player(const Player&) = delete;
			Player(D2Server& parent, std::string charname, std::string acctname, std::string client_ipaddr, int game_id)
				:parent_(parent), charname(charname), acctname(acctname), client_ipaddr(client_ipaddr), game_id(game_id),
				client_id(-1), char_class(-1), char_level(-1), in_game(false) { }

			void UpdateD2CS(bool enter, bool leave);
			void SendChatMessage(std::string sender, std::string msg, int color);
			void Kick();
			void Save(std::string& charsave);
			void SaveCharinfo(std::string& charinfo);
			void LeaveGame();
			void UpdateLadderInformation(int exp_low, int exp_high, int char_status);
		private:
			D2Server& parent_;
		};

		using PlayerRef = std::shared_ptr<Player>;
		PlayerRef CallbackFindPlayerToken(std::string charname, join_request_token token, int game_id);

		PlayerRef player(std::string charname) {
			std::lock_guard<std::mutex> guard(mutex_);
			if (players_.find(charname) == players_.end()) {
				return nullptr;
			}
			return players_[charname];
		}

		std::string realm_name() const {
			return net_manager_->d2cs_client().realm_name();
		}

		class GameInfo {
		public:
			int game_id;
			std::string game_name;

			GameInfo(const GameInfo&) = delete;
			GameInfo(D2Server& parent, int game_id, std::string game_name) :
				parent_(parent), game_id(game_id), game_name(game_name) { }

			std::string qualified_game_name() const {
				return game_name + "@" + parent_.realm_name();
			}
		private:
			D2Server& parent_;
		};

		using GameRef = std::shared_ptr<GameInfo>;
		GameRef game(int game_id) {
			std::lock_guard<std::mutex> guard(mutex_);
			if (games_.find(game_id) == games_.end()) {
				return nullptr;
			}
			return games_[game_id];
		}

	private:
		bool legacy_;
		D2GSINFO gs_info_;
		EVENTCALLBACKTABLE callback_table_;
		D2PoolManagerStrc* game_pool_managers_;
		Net::NetManagerRef net_manager_;
		std::unordered_map<join_request_token, JoinRequest> pending_join_requests_;
		
		std::unordered_map<std::string, PlayerRef> players_;
		std::unordered_map<int, GameRef> games_;
		std::mutex mutex_;

		using OnFrameEvent = std::function<void()>;
		std::deque<OnFrameEvent> on_frame_events_;
		void OnNextFrame(OnFrameEvent f);

		void PatchD2();
		int ServerInit();
		void ServerLoop();

	};
}

#endif