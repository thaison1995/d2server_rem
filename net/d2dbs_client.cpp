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
		net_client_.OnPacket(t_d2dbs_d2gs_save_data_reply_typecode, [this](std::string& s) {
			t_d2dbs_d2gs_save_data_reply reply;
			reply.ReadFromString(s);
			LOG(INFO) << "Save " << (reply.datatype == PROTO_DATATYPE::D2GS_DATA_CHARSAVE ? "charsave" : "charinfo") 
				<< " for " << reply.charname << " result: " 
				<< (reply.result == PROTO_SAVE_RESULT::D2DBS_SAVE_DATA_SUCCESS ? "SUCCESS" : "FAILURE");
		});
	}

	void D2DBSClient::handle_reply_packet(std::string& s)
	{
		t_d2gs_d2dbs_generic header;
		header.ReadFromString(s);

		int seqno = header.h.seqno;
		PendingCall call;
		{
			std::lock_guard<std::mutex> guard(mutex_);
			if (pending_calls_.find(seqno) == pending_calls_.end()) {
				LOG(WARNING) << "Got reply for packet seqno " << seqno << " but no pending call was found";
				return;
			}
			call = pending_calls_[seqno];
			pending_calls_.erase(seqno);
		}
		call(s);
	}

	void D2DBSClient::GetCharsaveDataAsync(std::string acctname, std::string charname, std::string realmname,
			get_data_success_handler success_handler, get_data_failure_handler failure_handler)
	{
		int seqno = ++seqno_;
		t_d2gs_d2dbs_get_data_request req;
		req.h.seqno = seqno;
		req.acctname = acctname;
		req.charname = charname;
		req.realmname = realmname;
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

	void D2DBSClient::SaveCharsaveAsync(std::string acctname, std::string charname, std::string realmname, 
		std::string ipaddr, std::string& charsave)
	{
		int seqno = ++seqno_;
		t_d2gs_d2dbs_save_data_request req;
		req.h.seqno = seqno;
		req.datatype = PROTO_DATATYPE::D2GS_DATA_CHARSAVE;
		req.datalen = charsave.length();
		req.acctname = acctname;
		req.charname = charname;
		req.realmname = realmname;
		req.data = charsave;
		net_client_.Send(req.WriteAsString());
		LOG(INFO) << "Request sent to save charsave for " << charname << "(*" << acctname << "), size is " << charsave.length();
	}

	void D2DBSClient::SaveCharinfoAsync(std::string acctname, std::string charname, std::string realmname, std::string& charinfo)
	{
		int seqno = ++seqno_;
		t_d2gs_d2dbs_save_data_request req;
		req.h.seqno = seqno;
		req.datatype = PROTO_DATATYPE::D2GS_DATA_PORTRAIT;
		req.datalen = charinfo.length();
		req.acctname = acctname;
		req.charname = charname;
		req.realmname = realmname;
		req.data = charinfo;
		net_client_.Send(req.WriteAsString());
		LOG(INFO) << "Request sent to save charinfo for " << charname << "(*" << acctname << "), size is " << charinfo.length();
	}

	void D2DBSClient::CharLockAsync(std::string acctname, std::string charname, std::string realmname, bool lock)
	{
		int seqno = ++seqno_;
		t_d2gs_d2dbs_char_lock req;
		req.h.seqno = seqno;
		req.lockstatus = lock ? 1 : 0;
		req.acctname = acctname;
		req.charname = charname;
		req.realmname = realmname;
		net_client_.Send(req.WriteAsString());
		LOG(INFO) << "Request sent to set charlock=" << lock << " for " << charname << "(*" << acctname << ")";
	}

	void D2DBSClient::UpdateLadderAsync(std::string acctname, std::string charname, int char_class, int char_level, 
		int exp_low, int exp_high, int char_status)
	{
		int seqno = ++seqno_;
		t_d2gs_d2dbs_update_ladder req;
		req.h.seqno = seqno;
		req.charlevel = char_level;
		req.charexplow = exp_low;
		req.charexphigh = exp_high;
		req.charclass = char_class;
		req.charstatus = char_status;
		req.charname = charname;
		net_client_.Send(req.WriteAsString());
		LOG(INFO) << "Sent ladder infromation for " << charname << "(*" << acctname << ")";
	}

}