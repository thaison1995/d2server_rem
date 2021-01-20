#ifndef INCLUDED_SERVER_GLOBAL_H
#define INCLUDED_SERVER_GLOBAL_H

#include <prometheus/registry.h>

enum ShutdownReason {
	NET_FAILURE,
	COMMAND
};

enum ShutdownAction {
	HALT,
	RESTART
};

void global_shutdown_handler(ShutdownReason reason, ShutdownAction action = ShutdownAction::HALT);

struct ServerConfig {
	std::string d2cs_host;
	int d2cs_port;
	std::string d2dbs_host;
	int d2dbs_port;

	bool logging_output_to_stderr;

	int gs_max_games;
	std::string gs_motd;
	bool gs_save_all_other_players;

	bool metrics_enabled;
	std::string metrics_exposer_host;
	int metrics_exposer_port;
};

const ServerConfig& server_config();

bool metrics_enabled();
std::shared_ptr<prometheus::Registry> metrics_registry();

#endif