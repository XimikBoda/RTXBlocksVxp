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


	void send_message(char* m);
	void send_command(char* m);

	void Set_Player_Rotation();
	void Set_Player_Position();

	void respawn();
};

