#ifndef INCLUDED_D2DBS_CLIENT_H
#define INCLUDED_D2DBS_CLIENT_H

#include "generated/d2dbs_proto.hpp"
#include "d2xs_client.hpp"

#include <map>
#include <mutex>

namespace Net {
	class D2DBSClient {
	public:
		D2DBSClient(const D2DBSClient&) = delete;
		D2DBSClient(asio::io_context& io_context,
			const std::string& server_host, const int server_port);

		bool connected() const {
			return connected_;
		}

		bool authed() const {
			return authed_;
		}

		enum class GetDataFailureReason: int {
			CharLocked,
			Unknown
		};

		using get_data_success_handler = std::function<void(bool allow_ladder, 
			int char_create_time, std::string charsave)>;
		using get_data_failure_handler = std::function<void(GetDataFailureReason reason)>;
		void GetCharsaveDataAsync(std::string acctname, std::string charname, 
			get_data_success_handler success_handler, get_data_failure_handler failure_handler);

		void SaveCharsaveAsync();
		void SaveCharinfoAsync();
		void UpdateLadderAsync();
		void CharLockAsync();
		void CloseSignalAsync();

	private:
		D2XSClient<t_d2dbs_d2gs_header> net_client_;
		bool connected_;
		std::atomic<int> seqno_;
		bool authed_;

		using PendingCall = std::function<void(std::string&)>;
		std::mutex mutex_;
		std::map<int, PendingCall> pending_calls_;

		void setup_handlers();
		void handle_reply_packet(std::string& s);

	};
}

#endif /* INCLUDED_D2DBS_D2GS_PROTOCOL_H */