#ifndef INCLUDED_SERVER_GLOBAL_H
#define INCLUDED_SERVER_GLOBAL_H

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

};

const ServerConfig& server_config();

#endif