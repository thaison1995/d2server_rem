#ifndef INCLUDED_NET_D2CS_CLIENT_H
#define INCLUDED_NET_D2CS_CLIENT_H

#include "d2xs_client.hpp"

#include "generated/d2cs_proto.hpp"

#include <prometheus/counter.h>

namespace Net {
	class D2CSClient {
	public:
		D2CSClient(const D2CSClient&) = delete;
		D2CSClient(asio::io_context& io_context,
			const std::string& server_host, const int server_port);

		bool connected() const {
			return connected_;
		}
		bool authed() const {
			return authed_;
		}
		std::string realm_name() const {
			return realm_name_;
		}

		void SetGSInfoAsync(int max_game, int game_flag);
		void UpdateGameInfoAsync(PROTO_UPDATEGAMEINFO_FLAG flag, int game_id, int char_level,
			int char_class, int addr, std::string charname);
		void CloseGameAsync(int game_id);

		using create_game_handler = std::function<bool(t_d2cs_d2gs_creategamereq& req, int& game_id)>;
		using join_game_handler = std::function<bool(t_d2cs_d2gs_joingamereq& req)>;
		void OnCreateGame(create_game_handler handler) {
			create_game_handler_ = handler;
		}
		void OnJoinGame(join_game_handler handler) {
			join_game_handler_ = handler;
		}

	private:
		D2XSClient<t_d2cs_d2gs_header> net_client_;
		bool connected_;
		std::atomic<int> seqno_;

		std::string realm_name_;
		int session_num_;
		bool authed_;

		create_game_handler create_game_handler_;
		join_game_handler join_game_handler_;

		void setup_handlers();
	};
}

#endif