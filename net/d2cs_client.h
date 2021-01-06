#ifndef INCLUDED_D2CS_CLIENT_H
#define INCLUDED_D2CS_CLIENT_H

#include "generated/d2cs_proto.hpp"
#include "d2xs_client.hpp"

#include <future>

namespace Net {
	class D2CSClient {
	public:
		D2CSClient(const D2CSClient&) = delete;
		D2CSClient(asio::io_context& io_context,
			const std::string& server_host, const int server_port)
			: net_client_("d2cs", io_context, server_host, server_port) {
			connected_ = false;
			net_client_.OnError([&](std::error_code ec) {
				connected_ = false;
				throw std::exception("D2CS Connection Error");
			});
			net_client_.Connect([&]() {
				connected_ = true;
				std::string bnclass;
				bnclass.resize(1);
				bnclass[0] = (char)CONNECT_CLASS_D2GS_TO_D2CS;
				net_client_.Send(bnclass);
			});
		}

		bool connected() const {
			return connected_;
		}

		void SetGSInfoAsync(int max_game, int game_flag);
		void UpdateGameInfoAsync(int flag, int game_id, int char_level, int char_class, int addr);
		void CloseGameAsync(int game_id);

		std::future<std::string> GetConfAsync();

	private:
		D2XSClient<t_d2cs_d2gs_header> net_client_;
		bool connected_;
	};
}

#endif