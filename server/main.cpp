#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>

#include "net/net_manager.h"
#include "server/d2server.h"

int main(int argc, char** argv)
{
	//void* mem = VirtualAlloc((void*)0x400000, 0x10000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	FLAGS_logtostderr = 1;
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "Initializing D2Server REmastered";

	Net::NetManagerRef net_manager =
		std::make_shared<Net::NetManager>("172.24.103.57", 6130, "127.0.0.1", 6114);

	LOG(INFO) << "Bringing up d2server";

	Server::D2Server server(false, net_manager);
	server.Run();
}
