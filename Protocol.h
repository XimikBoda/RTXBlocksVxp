#pragma once
namespace Protocol
{
	void init();
	void update();
	void connect();
	void tcp_connect_event();

	void open_packet(int id);
	void open_packets();
	void dns_event();
};

