#include "net_manager.h"
#include "server/global.h"

namespace Net {
	NetManager::NetManager(const std::string& d2cs_host, const int d2cs_port,
		const std::string& d2dbs_host, const int d2dbs_port) :
		io_context_(), d2cs_client_(io_context_, d2cs_host, d2cs_port),
		d2dbs_client_(io_context_, d2dbs_host, d2dbs_port) {
		asio_thread_ = std::thread([this]() {
			try {
				io_context_.run();
			}
			catch (const std::exception& e) {
				LOG(ERROR) << "Exception in asio thread, shutting down";
				global_shutdown_handler(ShutdownReason::NET_FAILURE);
			}
		});
		LOG(INFO) << "NetManager initialized";
		int n = 0;
		while (!d2cs_client_.connected() && n < 10) {
			LOG(INFO) << "Waiting d2cs client to connect..";
			n++;
			Sleep(1000);
		}
		while (!d2dbs_client_.connected() && n < 10) {
			LOG(INFO) << "Waiting d2dbs client to connect..";
			n++;
			Sleep(1000);
		}
		if (!d2cs_client_.connected()) {
			LOG(ERROR) << "Not connected to D2CS";
			throw std::exception("Not connected to D2CS");
		}
		if (!d2dbs_client_.connected()) {
			LOG(ERROR) << "Not connected to D2DBS";
			throw std::exception("Not connected to D2DBS");
		}
	}
}