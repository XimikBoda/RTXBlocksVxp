#pragma once

const int tcp_in_buf_size = 10240*2, tcp_out_buf_size = 10240*2;
const int udp_in_buf_size = 10240*2, udp_out_buf_size = 10240*2;

namespace Sock
{
	void init();
	void tcp_connect(const char* host, const unsigned short port);
	void udp_connect(const unsigned short port);
	void get_ip_by_dns(const char* host);
	void update();
	void deinit();
};

