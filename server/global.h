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

#endif