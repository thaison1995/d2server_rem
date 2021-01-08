#include "d2dbs_client.h"

#include "server/global.h"

namespace Net {

	D2DBSClient::D2DBSClient(asio::io_context& io_context,
		const std::string& server_host, const int server_port)
		: net_client_("d2dbs", io_context, server_host, server_port) {
		connected_ = false;
		authed_ = false;
		net_client_.OnError([&](std::error_code ec) {
			connected_ = false;
			throw std::exception("D2DBS Connection Error");
		});
		setup_handlers();
		seqno_ = 0;
		net_client_.Connect([&]() {
			connected_ = true;
			std::string bnclass;
			bnclass.resize(1);
			bnclass[0] = (char)CONNECT_CLASS_D2GS_TO_D2DBS;
			net_client_.Send(bnclass);
		});
	}

	void D2DBSClient::setup_handlers()
	{
		using namespace std::placeholders;
		auto reply_packet_handler = std::bind(&D2DBSClient::handle_reply_packet, this, _1);

		net_client_.OnPacket(t_d2dbs_d2gs_get_data_reply_typecode, reply_packet_handler);
	}

	void D2DBSClient::handle_reply_packet(std::string& s)
	{
		t_d2gs_d2dbs_generic header;
		header.ReadFromString(s);

		int seqno = ntohl(header.h.seqno);

		{
			std::lock_guard<std::mutex> guard(mutex_);
			if (pending_calls_.find(seqno) == pending_calls_.end()) {
				LOG(WARNING) << "Got reply for packet seqno " << seqno << " but no pending call was found";
				return;
			}
			pending_calls_[seqno](s);
			//pending_calls_.erase(seqno);
		}
	}

	void D2DBSClient::GetCharsaveDataAsync(std::string acctname, std::string charname,
			get_data_success_handler success_handler, get_data_failure_handler failure_handler)
	{
		int seqno = ++seqno_;
		t_d2gs_d2dbs_get_data_request req;
		req.h.seqno = htonl(seqno);
		req.acctname = acctname;
		req.charname = charname;
		req.datatype = PROTO_DATATYPE::D2GS_DATA_CHARSAVE;

		{
			std::lock_guard<std::mutex> guard(mutex_);
			pending_calls_[seqno] = [charname, acctname, success_handler, failure_handler](std::string& s) {
				t_d2dbs_d2gs_get_data_reply reply;
				reply.ReadFromString(s);
				if (reply.result == PROTO_GET_DATA_RESULT::D2DBS_GET_DATA_SUCCESS) {
					success_handler(reply.allowladder, reply.charcreatetime, reply.data);
					LOG(INFO) << "Successfully retrieved charsave for " << charname << "(*" << acctname << ")";
				}
				else if (reply.result == PROTO_GET_DATA_RESULT::D2DBS_GET_DATA_CHARLOCKED) {
					failure_handler(GetDataFailureReason::CharLocked);
					LOG(WARNING) << "Failed to get charsave for " << charname << "(*" << acctname << "): Char is locked";
				} 
				else {
					failure_handler(GetDataFailureReason::Unknown);
					LOG(ERROR) << "Failed to get charsave for " << charname << "(*" << acctname << "): No reason was given";
				}
			};
		}
		net_client_.Send(req.WriteAsString());
		LOG(INFO) << "Request sent to get charsave for " << charname << "(*" << acctname << ")";
	}

}