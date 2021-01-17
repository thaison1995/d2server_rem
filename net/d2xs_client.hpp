#ifndef INCLUDED_NET_D2CX_CLIENT_HPP
#define INCLUDED_NET_D2CX_CLIENT_HPP

#include <thread>
#include <functional>
#include <string>

#define ASIO_STANDALONE
#include <asio.hpp>

#include <deque>
#include <map>

#include "server/logger.h"

#include "bn_types.h"

namespace Net {
	using asio::ip::tcp;

	template<typename T>
	class D2XSResponse {
	public:
		const static size_t response_header_length = sizeof(T);

		D2XSResponse() {
			memset(&header_, 0, sizeof(T));
		}
		bool decode_header() {
			body_.resize(body_size());
			return true;
		}
		size_t header_size() const {
			return response_header_length;
		}
		size_t body_size() const {
			return header_.size - sizeof(T);
		}
		char* header_data() {
			return (char*)&header_;
		}
		char* body_data() {
			if (body_size() > 0) {
				return &body_.front();
			}
			else {
				return nullptr;
			}
		}
		bn_short packet_type() {
			return header_.type;
		}
		std::string packet() const {
			size_t packet_size = sizeof(T) + body_size();
			std::string s;
			s.resize(packet_size);

			memcpy(&s.front(), &header_, sizeof(T));
			if (body_size() > 0) {
				memcpy(&s.front() + sizeof(T), &body_.front(), body_size());
			}

			return s;
		}
	private:
		T header_;
		std::string body_;
	};


	template<typename T>
	class D2XSClient {
	public:
		using on_packet_handler = std::function<void(std::string&)>;
		using on_connect_handler = std::function<void()>;
		using on_error_handler = std::function<void(std::error_code)>;
		using d2xs_request = std::string;

		D2XSClient(const D2XSClient&) = delete;
		D2XSClient(std::string client_name, asio::io_context& io_context, 
			const std::string& server_host, const int server_port) :
			client_name_(client_name), io_context_(io_context), socket_(io_context) {
			on_packet_handlers_.resize(0xFF);

			std::error_code ec;
			asio::ip::address ip_address = asio::ip::address::from_string(server_host, ec);
			endpoint_ = asio::ip::tcp::endpoint(ip_address, server_port);
			LOG(INFO) << "[" << client_name_ << "] Initialized client to " << server_host << ":" << server_port;
		}

		void OnPacket(bn_short packet_type, on_packet_handler handler) {
			on_packet_handlers_[packet_type] = handler;
		}

		void OnError(on_error_handler handler) {
			on_error_handler_ = handler;
		}

		void Connect(on_connect_handler callback) {
			do_connect(endpoint_, callback);
		}

		void Send(d2xs_request request) {
			asio::post(io_context_,
				[this, request]()
			{
				bool write_in_progress = !request_queue_.empty();
				request_queue_.push_back(request);
				if (!write_in_progress)
				{
					do_write();
				}
			});
		}

		void Close() {
			LOG(INFO) << "[" << client_name_ << "] Closing connection";
			asio::post(io_context_, [this]() { socket_.close(); });
		}

	private:
		using request_queue = std::deque<d2xs_request>;

		asio::ip::tcp::endpoint endpoint_;
		asio::io_context& io_context_;
		tcp::socket socket_;
		request_queue request_queue_;
		D2XSResponse<T> response_;
		std::vector<on_packet_handler> on_packet_handlers_;
		on_error_handler on_error_handler_;

		std::string client_name_;

	private:
		void do_connect(const asio::ip::tcp::endpoint& endpoint, on_connect_handler callback)
		{
			LOG(INFO) << "[" << client_name_ << "] Connecting";
			socket_.async_connect(endpoint_, [this, callback](std::error_code ec) {
				if (!ec)
				{
					LOG(INFO) << "[" << client_name_ << "] Connected";
					if (callback) {
						callback();
					}
					do_read_header();
				}
				else {
					LOG(ERROR) << "[" << client_name_ << "] Error while connecting: (" << ec.value() << ") " << ec.message();
					if (on_error_handler_) {
						on_error_handler_(ec);
					}
				}
			});
		}

		void do_read_header()
		{
			asio::async_read(socket_,
				asio::buffer(response_.header_data(), response_.header_size()),
				[this](std::error_code ec, std::size_t /*length*/)
			{
				if (!ec && response_.decode_header())
				{
					do_read_packet();
				}
				else
				{
					LOG(ERROR) << "[" << client_name_ << "] Error while reading header: (" << ec.value() << ") " << ec.message();
					if (on_error_handler_) {
						on_error_handler_(ec);
					}
					socket_.close();
				}
			});
		}

		void do_read_packet()
		{
			asio::async_read(socket_,
				asio::buffer(response_.body_data(), response_.body_size()),
				[this](std::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					// handle pkt
					bn_short packet_type = response_.packet_type();
					std::string packet = response_.packet();
					LOG(INFO) << "[" << client_name_ << "] Got packet type " << packet_type << " (size: " << packet.length() << ")";

					if (on_packet_handlers_[packet_type]) {
						// make a copy, otherwise buffer can be tempered by one of those following responses
						on_packet_handlers_[packet_type](packet);
					}
					else {
						LOG(WARNING) << "[" << client_name_ << "] No handler for packet type " << packet_type;
					}

					do_read_header();
				}
				else
				{
					LOG(ERROR) << "[" << client_name_ << "] Error while reading packet: (" << ec.value() << ") " << ec.message();
					if (on_error_handler_) {
						on_error_handler_(ec);
					}
					socket_.close();
				}
			});
		}

		void do_write()
		{
			asio::async_write(socket_,
				asio::buffer(&request_queue_.front().front(),
					request_queue_.front().length()),
				[this](std::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					LOG(INFO) << "[" << client_name_ << "] Sent " << request_queue_.front().length() << " bytes";
					request_queue_.pop_front();
					if (!request_queue_.empty())
					{
						do_write();
					}
				}
				else
				{
					LOG(ERROR) << "[" << client_name_ << "] Error while writing: (" << ec.value() << ") " << ec.message();
					if (on_error_handler_) {
						on_error_handler_(ec);
					}
					socket_.close();
				}
			});
		}
	};

}

#endif