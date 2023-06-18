#include "Sock.h"
#include "Protocol.h"
#include <corecrt_malloc.h>
#include <SFML/Network.hpp>

unsigned char* tcp_in_buf = 0, * tcp_out_buf = 0;
int tcp_in_buf_pos = 0, tcp_out_buf_pos = 0;

unsigned char* udp_in_buf = 0, * udp_out_buf = 0;
int udp_in_buf_pos = 0, udp_out_buf_pos = 0;

static int tcp_handle = -1;
static int udp_handle = -1;

bool sock_is_conected = 0;

sf::TcpSocket tcp_sock;
sf::UdpSocket udp_sock;

sf::IpAddress udp_addr;
unsigned short udp_port = 0;

namespace Sock
{
	void init() {
		tcp_in_buf = (unsigned char*)malloc(tcp_in_buf_size);
		tcp_out_buf = (unsigned char*)malloc(tcp_out_buf_size);
		udp_in_buf = (unsigned char*)malloc(udp_in_buf_size);
		udp_out_buf = (unsigned char*)malloc(udp_out_buf_size);
	}
	//// TCP
	void tcp_connect(const char* host, const unsigned short port) {
		sock_is_conected = 0;

		tcp_sock.connect(sf::IpAddress(host), port);

		sock_is_conected = 1;
		Protocol::tcp_connect_event();
	}

	void tcp_update() {
			size_t rec = 0;
			tcp_sock.setBlocking(0);
			tcp_sock.receive(tcp_in_buf + tcp_in_buf_pos, tcp_in_buf_size - tcp_in_buf_pos - 100, rec);
			if (rec > 0)
				tcp_in_buf_pos += rec;

			if (tcp_out_buf_pos > 0) {
				size_t snd = 0;
				tcp_sock.setBlocking(1);
				tcp_sock.send(tcp_out_buf, tcp_out_buf_pos, snd);
				if (snd > 0) {
					if (snd != tcp_out_buf_pos)
						memmove(tcp_out_buf, tcp_out_buf + snd, tcp_out_buf_pos);
					tcp_out_buf_pos -= snd;
				}
			}
	}

	////  UDP
	void udp_connect(const unsigned short port) {
		udp_port = port;
	}

	void udp_update() {
		sf::IpAddress in_addr;
		unsigned short in_port;
			size_t rec = 0;
			udp_sock.setBlocking(0);
			udp_sock.receive(udp_in_buf + udp_in_buf_pos, udp_in_buf_size - udp_in_buf_pos - 100, rec, in_addr, in_port);
			if (rec > 0)
				udp_in_buf_pos += rec;

			if (udp_out_buf_pos > 0) {
				int snd = 0;
				udp_sock.setBlocking(1);
				auto st = udp_sock.send(udp_out_buf, udp_out_buf_pos, udp_addr, udp_port);
				if (st == sf::Socket::Done)
					snd = udp_out_buf_pos;
				if (snd > 0) {
					if (snd != udp_out_buf_pos)
						memmove(udp_out_buf, udp_out_buf + snd, udp_out_buf_pos);
					udp_out_buf_pos -= snd;
				}
			}
		//}
	}
	//// DNS
	void get_ip_by_dns(const char* host) {
		udp_addr = host;
		Protocol::dns_event();
	}
	/// Update all
	void update() {
		tcp_update();
		udp_update();
	}

	/// Deinit 
	void deinit() {
		if (tcp_handle >= 0) {
			tcp_handle = -1;
		}
		if (udp_handle >= 0) {
			udp_handle = -1;
		}
		free((void*)tcp_in_buf);
		free((void*)tcp_out_buf);
		free((void*)udp_in_buf);
		free((void*)udp_out_buf);
		tcp_in_buf = tcp_out_buf = udp_in_buf = udp_out_buf = 0;
		sock_is_conected = 0;
	}
};
