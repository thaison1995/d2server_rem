#include "d2cs_client.h"

#include "server/global.h"

namespace Net {

	D2CSClient::D2CSClient(asio::io_context& io_context,
		const std::string& server_host, const int server_port)
		: net_client_("d2cs", io_context, server_host, server_port) {
		connected_ = false;
		net_client_.OnError([&](std::error_code ec) {
			connected_ = false;
			throw std::exception("D2CS Connection Error");
		});
		setup_handlers();
		seqno_ = 0;
		net_client_.Connect([&]() {
			connected_ = true;
			std::string bnclass;
			bnclass.resize(1);
			bnclass[0] = (char)CONNECT_CLASS_D2GS_TO_D2CS;
			net_client_.Send(bnclass);
		});
	}

	void D2CSClient::setup_handlers() {
		net_client_.OnPacket(t_d2cs_d2gs_authreq_typecode, [this](std::string s) {
			t_d2cs_d2gs_authreq req;
			req.ReadFromString(s);
			realm_name_ = req.realm_name;
			session_num_ = req.sessionnum;
			LOG(INFO) << "D2CS Realm Name: " << realm_name_;
			LOG(INFO) << "D2CS Session Num: " << session_num_;

			t_d2gs_d2cs_authreply reply;
			reply.h.type = t_d2gs_d2cs_authreply_typecode;
			reply.h.size = bn_ntohs(sizeof(t_d2gs_d2cs_authreply));
			reply.h.seqno = req.h.seqno;
			reply.version = 0x01130000;
			reply.checksum = 0;
			net_client_.Send(reply.WriteAsString());
		});

		net_client_.OnPacket(t_d2cs_d2gs_authreply_typecode, [this](std::string s) {
			t_d2cs_d2gs_authreply reply;
			reply.ReadFromString(s);
			if (reply.reply != PROTO_AUTHREPLY::D2CS_D2GS_AUTHREPLY_SUCCEED) {
				LOG(FATAL) << "Failed to auth with d2cs, reply: " << reply.reply;
				throw std::exception("Failed to auth with d2cs");
			}
			LOG(INFO) << "Successfully authed with d2cs";

			authed_ = true;
		});

		net_client_.OnPacket(t_d2cs_d2gs_setconffile_typecode, [this](std::string s) {
			t_d2cs_d2gs_setconffile req;
			req.ReadFromString(s);
			LOG(INFO) << "Received conf from d2cs (" << req.conf.length() << " bytes)";

		});

		net_client_.OnPacket(t_d2cs_d2gs_setinitinfo_typecode, [this](std::string s) {
			t_d2cs_d2gs_setinitinfo req;
			req.ReadFromString(s);
			LOG(INFO) << "Received initinfo from d2cs: time=" << req.time << ", gs_id=" << req.gs_id << ", ac_version=" << req.ac_version;
		});

		net_client_.OnPacket(t_d2cs_d2gs_echoreq_typecode, [this](std::string s) {
			t_d2cs_d2gs_echoreq req;
			req.ReadFromString(s);
			t_d2gs_d2cs_echoreply reply;
			reply.h.seqno = req.h.seqno;
			net_client_.Send(req.WriteAsString());
		});

		net_client_.OnPacket(t_d2cs_d2gs_control_typecode, [this](std::string s) {
			t_d2cs_d2gs_control req;
			req.ReadFromString(s);
			LOG(INFO) << "Command from D2CS: " << req.cmd;
			if (req.cmd == PROTO_CONTROL_CMD::D2CS_D2GS_CONTROL_CMD_RESTART) {
				global_shutdown_handler(ShutdownReason::COMMAND, ShutdownAction::RESTART);
			}
			if (req.cmd == PROTO_CONTROL_CMD::D2CS_D2GS_CONTROL_CMD_SHUTDOWN) {
				global_shutdown_handler(ShutdownReason::COMMAND, ShutdownAction::HALT);
			}
		});

		net_client_.OnPacket(t_d2cs_d2gs_creategamereq_typecode, [this](std::string s) {
			t_d2cs_d2gs_creategamereq req;
			req.ReadFromString(s);
			// TODO
		});

		net_client_.OnPacket(t_d2cs_d2gs_joingamereq_typecode, [this](std::string s) {
			t_d2cs_d2gs_joingamereq req;
			req.ReadFromString(s);
			// TODO
		});
	}

	void D2CSClient::SetGSInfoAsync(int max_game, int game_flag) {
		LOG(INFO) << "Setting number of games to " << max_game;
		t_d2gs_d2cs_setgsinfo req;
		req.h.seqno = seqno_++;
		req.maxgame = max_game;
		req.gameflag = game_flag;
		net_client_.Send(req.WriteAsString());
	}

	void D2CSClient::UpdateGameInfoAsync(PROTO_UPDATEGAMEINFO_FLAG flag, int game_id, int char_level, int char_class, int addr) {
		t_d2gs_d2cs_updategameinfo req;
		req.h.seqno = seqno_++;
		req.flag = flag;
		req.gameid = game_id;
		req.charlevel = char_level;
		req.charclass = char_class;
		req.addr = addr;
		net_client_.Send(req.WriteAsString());
	}

	void D2CSClient::CloseGameAsync(int game_id) {
		t_d2gs_d2cs_closegame req;
		req.h.seqno = seqno_++;
		req.gameid = game_id;
		net_client_.Send(req.WriteAsString());
	}

	void D2CSClient::OnCreateGame(create_game_handler handler)
	{
	}

	void D2CSClient::OnJoinGame(join_game_handler handler)
	{
	}

}