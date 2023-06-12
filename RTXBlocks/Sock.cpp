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

//static vm_sockaddr_struct udp_addr;

bool sock_is_conected = 0;

sf::TcpSocket tcp_sock;
sf::UdpSocket udp_sock;

sf::IpAddress udp_addr;
unsigned short udp_port = 0;

namespace Sock
{
	void init() {
		//udp_addr.addr_len = 0;
		tcp_in_buf = (unsigned char*)malloc(tcp_in_buf_size);
		tcp_out_buf = (unsigned char*)malloc(tcp_out_buf_size);
		udp_in_buf = (unsigned char*)malloc(udp_in_buf_size);
		udp_out_buf = (unsigned char*)malloc(udp_out_buf_size);

		//if (tcp_in_buf == 0 || tcp_out_buf == 0 || udp_in_buf == 0 || udp_out_buf == 0)
		//	show_error_and_exit("Sock malloc error");
	}
	//// TCP
	/*void tcp_callback(VMINT handle, VMINT event) {
		switch (event) {
		case VM_TCP_EVT_CONNECTED:
			sock_is_conected = 1;
			Protocol::tcp_connect_event();
			break;
		case VM_TCP_EVT_PIPE_BROKEN:
			show_error_and_exit("Tcp pipe broken");
			break;
		case VM_TCP_EVT_HOST_NOT_FOUND:
			show_error_and_exit("Tcp host not found");
			break;
		case VM_TCP_EVT_PIPE_CLOSED:
			show_error_and_exit("Tcp pipe closed");
			break;
		}
	}*/

	void tcp_connect(const char* host, const unsigned short port) {
		//if (tcp_handle >= 0) { show_error_and_exit("Tcp already opened"); return; }

		sock_is_conected = 0;

		//tcp_handle = vm_tcp_connect(host, port, 1, tcp_callback);
		tcp_sock.connect(sf::IpAddress(host), port);

		sock_is_conected = 1;
		Protocol::tcp_connect_event();

		//if (tcp_handle < 0) show_error_and_exit("Tcp open error");
	}

	void tcp_update() {
		//if (tcp_handle >= 0) {
			size_t rec = 0;
			tcp_sock.setBlocking(0);
			tcp_sock.receive(tcp_in_buf + tcp_in_buf_pos, tcp_in_buf_size - tcp_in_buf_pos - 100, rec);
			// = vm_tcp_read(tcp_handle, tcp_in_buf + tcp_in_buf_pos, tcp_in_buf_size - tcp_in_buf_pos - 100);
			//if (rec < 0)
			//	show_error_and_exit("Tcp reed error");
			if (rec > 0)
				tcp_in_buf_pos += rec;

			if (tcp_out_buf_pos > 0) {
				size_t snd = 0;// = vm_tcp_write(tcp_handle, tcp_out_buf, tcp_out_buf_pos);
				tcp_sock.setBlocking(1);
				tcp_sock.send(tcp_out_buf, tcp_out_buf_pos, snd);
				//if (snd < 0)
				//	show_error_and_exit("Tcp write error");
				if (snd > 0) {
					if (snd != tcp_out_buf_pos)
						memmove(tcp_out_buf, tcp_out_buf + snd, tcp_out_buf_pos);
					tcp_out_buf_pos -= snd;
				}
			}
		//}
	}

	////  UDP
	/*void udp_callback(VMINT hdl, VMINT event) {
		switch (event) {
		case VM_UDP_EVT_READ:
			break;
		case VM_UDP_EVT_PIPE_BROKEN:
			show_error_and_exit("Udp pipe broken");
			break;
		case VM_UDP_EVT_PIPE_CLOSED:
			show_error_and_exit("Udp pipe closed");
			break;
		}
	}*/

	void udp_connect(const unsigned short port) {
		//if (udp_handle >= 0) {/*show_error_and_exit("Udp already opened");*/ return; }

		//if (udp_addr.addr_len == 0) { show_error_and_exit("Udp address error"); return; }

		udp_port = port;

		//udp_addr.port = port;
		//udp_handle = vm_udp_create(rand() % 65000 + 1, 1, udp_callback, 0);
		

		//if (udp_handle < 0) show_error_and_exit("Udp open error");
	}

	void udp_update() {
		sf::IpAddress in_addr;
		unsigned short in_port;
		//if (udp_handle >= 0) {
			size_t rec = 0;// vm_udp_recvfrom(udp_handle, udp_in_buf + udp_in_buf_pos, udp_in_buf_size - udp_in_buf_pos - 100, &tmp_addr);
			udp_sock.setBlocking(0);
			udp_sock.receive(udp_in_buf + udp_in_buf_pos, udp_in_buf_size - udp_in_buf_pos - 100, rec, in_addr, in_port);
			//if (rec < 0)
			//	show_error_and_exit("Udp reed error");
			if (rec > 0)
				udp_in_buf_pos += rec;

			if (udp_out_buf_pos > 0) {
				int snd = 0;// vm_udp_sendto(udp_handle, udp_out_buf, udp_out_buf_pos, &udp_addr);
				udp_sock.setBlocking(1);
				auto st = udp_sock.send(udp_out_buf, udp_out_buf_pos, udp_addr, udp_port);
				if (st == sf::Socket::Done)
					snd = udp_out_buf_pos;
				//if (snd < 0)
				//	show_error_and_exit("Udp write error");
				if (snd > 0) {
					if (snd != udp_out_buf_pos)
						memmove(udp_out_buf, udp_out_buf + snd, udp_out_buf_pos);
					udp_out_buf_pos -= snd;
				}
			}
		//}
	}
	//// DNS
	/*int d_callback(vm_soc_dns_result* d) {
		udp_addr.addr_len = d->num * 4;
		(*(unsigned int*)udp_addr.addr) = d->address[0];
		Protocol::dns_event();
		return 0;
	}*/

	void get_ip_by_dns(const char* host) {
		/*vm_soc_dns_result dns;

		int u = vm_soc_get_host_by_name(1, host, &dns, d_callback);

		if (u == VM_E_SOC_SUCCESS) {
			udp_addr.addr_len = dns.num * 4;
			(*(unsigned int*)udp_addr.addr) = dns.address[0];
			Protocol::dns_event();
		}
		else if (u != VM_E_SOC_WOULDBLOCK)
			show_error_and_exit("DNS error");*/
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
			//vm_tcp_close(tcp_handle);
			tcp_handle = -1;
		}
		if (udp_handle >= 0) {
			//vm_udp_close(udp_handle);
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
