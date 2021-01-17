#include "server/logger.h"
#include "net/net_manager.h"
#include "server/d2server.h"
#include "global.h"

#include <yaml-cpp/yaml.h>
#ifdef _DEBUG
#pragma comment(lib, "yaml-cppd.lib")
#else
#pragma comment(lib, "yaml-cpp.lib")
#endif

#include <fstream>

void global_shutdown_handler(ShutdownReason reason, ShutdownAction action) {
	LOG(INFO) << "Shutting down because: " << reason;
	ExitProcess(0);
}

static ServerConfig g_server_config;

const ServerConfig& server_config() {
	return g_server_config;
}

void load_server_config(std::string& filename) {
	YAML::Node config = YAML::LoadFile(filename);
	g_server_config.d2cs_host = config["d2cs"]["host"].as<std::string>();
	g_server_config.d2cs_port = config["d2cs"]["port"].as<int>();
	g_server_config.d2dbs_host = config["d2dbs"]["host"].as<std::string>();
	g_server_config.d2dbs_port = config["d2dbs"]["port"].as<int>();
	g_server_config.logging_output_to_stderr = config["logging"]["output_to_stderr"].as<bool>();
	g_server_config.gs_max_games = config["gs"]["max_games"].as<int>();
	g_server_config.gs_motd = config["gs"]["motd"].as<std::string>();
}

int main(int argc, char** argv)
{
	std::string config_file = "d2server_config.yaml";
	if (argc > 1) {
		config_file = argv[1];
	}
	load_server_config(config_file);

	//void* mem = VirtualAlloc((void*)0x400000, 0x10000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	FLAGS_log_dir = ".";
	if (server_config().logging_output_to_stderr) {
		FLAGS_alsologtostderr = 1;
	}
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "Using config file: " << config_file;

	LOG(INFO) << "Initializing D2Server REmastered";

	Net::NetManagerRef net_manager =
		std::make_shared<Net::NetManager>(server_config().d2cs_host, server_config().d2cs_port, 
			server_config().d2dbs_host, server_config().d2dbs_port);

	LOG(INFO) << "Bringing up d2server";

	Server::D2Server server(net_manager);

	// example for packet filtering
	server.RegisterGamePacketFilter((char)0x01, [&](Server::D2Server::PlayerRef player, UnitAny* pUnit, 
		const char* packet, Game* pGame, int len) {
		player->SendChatMessage("admin", "Packet received: 0x01", D2COLOR_ID_BLUE);
	});

	server.Run();
}
