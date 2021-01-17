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

#include <prometheus/registry.h>
#include <prometheus/exposer.h>
#ifdef _DEBUG
#pragma comment(lib, "prometheus-cpp-cored.lib")
#pragma comment(lib, "prometheus-cpp-pulld.lib")
#else
#pragma comment(lib, "prometheus-cpp-core.lib")
#pragma comment(lib, "prometheus-cpp-pull.lib")
#endif

#include <sstream>

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
	g_server_config.metrics_enabled = config["metrics"]["enabled"].as<bool>();
	g_server_config.metrics_exposer_host = config["metrics"]["exposer_host"].as<std::string>();
	g_server_config.metrics_exposer_port = config["metrics"]["exposer_port"].as<int>();
}

std::shared_ptr<prometheus::Registry> g_registry;

bool metrics_enabled()
{
	return server_config().metrics_enabled;
}

std::shared_ptr<prometheus::Registry> metrics_registry() {
	return g_registry;
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

	std::unique_ptr<prometheus::Exposer> exposer;
	if (server_config().metrics_enabled) {
		std::stringstream ss;
		ss << server_config().metrics_exposer_host;
		ss << ":";
		ss << server_config().metrics_exposer_port;
		std::string endpoint = ss.str();

		LOG(INFO) << "Enableing metrics exposer on " << endpoint;
		exposer = std::make_unique<prometheus::Exposer>(endpoint);
		g_registry = std::make_shared<prometheus::Registry>();
		exposer->RegisterCollectable(g_registry);
	}
	else {
		exposer = nullptr;
	}
	LOG(INFO) << "Initializing D2Server REmastered";

	Net::NetManagerRef net_manager =
		std::make_shared<Net::NetManager>(server_config().d2cs_host, server_config().d2cs_port, 
			server_config().d2dbs_host, server_config().d2dbs_port);

	LOG(INFO) << "Bringing up d2server";

	Server::D2Server server(net_manager);

	// example for packet filtering
	/*
	server.RegisterGamePacketFilter((char)0x01, [&](Server::D2Server::PlayerRef player, UnitAny* pUnit,
		const char* packet, Game* pGame, int len) -> Server::D2Server::PacketFilterAction {
		player->SendChatMessage("admin", "Packet received: 0x01", D2COLOR_ID_BLUE);
		return Server::D2Server::PacketFilterAction::PASS;
	});
	server.RegisterGamePacketFilter((char)0x59, [&](Server::D2Server::PlayerRef player, UnitAny* pUnit,
		const char* packet, Game* pGame, int len) -> Server::D2Server::PacketFilterAction {
		return Server::D2Server::PacketFilterAction::KICK;
	});
	*/

	server.Run();
}
