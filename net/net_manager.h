#ifndef INCLUDED_NET_MANAGER_H
#define INCLUDED_NET_MANAGER_H

#include "d2cs_client.h"
#include "d2dbs_client.h"

#include <string>

#undef ERROR
#include <glog/logging.h>

#include <thread>
#include <memory>

namespace Net {
	class NetManager {
	public:
		NetManager(const NetManager&) = delete;
		NetManager(const std::string& d2cs_host, const int d2cs_port,
			const std::string& d2dbs_host, const int d2dbs_port);

		D2CSClient& d2cs_client() {
			return d2cs_client_;
		}

		D2DBSClient& d2dbs_client() {
			return d2dbs_client_;
		}
	private:
		asio::io_context io_context_;
		D2CSClient d2cs_client_;
		D2DBSClient d2dbs_client_;
		std::thread asio_thread_;
	};

	using NetManagerRef = std::shared_ptr<Net::NetManager>;
}

#endif