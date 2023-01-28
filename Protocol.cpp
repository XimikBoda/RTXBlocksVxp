#include "Protocol.h"
#include "PacketMaker.h"
#include "PacketOpener.h"
#include "Sock.h"
#include "vmsim.h"
#include <cstdlib>
#include "string.h"

extern bool sock_is_conected;
extern unsigned char *tcp_in_buf,  *tcp_out_buf;
extern int tcp_in_buf_pos, tcp_out_buf_pos;

extern unsigned char *udp_in_buf,  *udp_out_buf;
extern int udp_in_buf_pos, udp_out_buf_pos;

#define TO_TCP tcp_out_buf, tcp_out_buf_pos
#define TO_UDP udp_out_buf, udp_out_buf_pos

//temp
const char* r_ip = "127.0.0.1";
unsigned short r_tcp_port = 25501;
unsigned short r_udp_port = 25502;
const char* s_ip = "127.0.0.1";
unsigned short s_port = 25565;
const char* Nickname = "MoDis";
int my_id = -1;

namespace Protocol
{
	
	void init(){}
	void update(){
		Sock::update();
		open_packets();
	}
	void connect(/*...*/){
		Sock::tcp_connect(r_ip, r_tcp_port);
		Sock::get_ip_by_dns(r_ip);
	}

	void open_packets(){
		int i=0;
		for(; i<tcp_in_buf_pos;){
			int size_size = 0;
			int size = PacketOpener::read_VarInt_by_adress(tcp_in_buf+i,&size_size);
			if(size_size+size+i<=tcp_in_buf_pos){
				open_packet(PacketOpener::open(tcp_in_buf+i, size, size_size));
			
				i+=size_size+size;
			}else
				break;
		}
		if(i!=0&&tcp_in_buf_pos!=i)
			memmove(tcp_in_buf, tcp_in_buf+i, tcp_in_buf_pos-i);
		tcp_in_buf_pos-=i;
	}

	void open_packet(int id){
		switch(id){
			case 1:
				{//custom format
					my_id = PacketOpener::read_unsigned_short();

					unsigned short s_my_id = my_id;
					memcpy(udp_out_buf + udp_out_buf_pos, &s_my_id, 2);
					memcpy(udp_out_buf + udp_out_buf_pos + 2, &s_my_id, 2);
					memcpy(udp_out_buf + udp_out_buf_pos + 4, &s_my_id, 2);
					memcpy(udp_out_buf + udp_out_buf_pos + 6, &s_my_id, 2);
					udp_out_buf_pos += 8;
				}
				break;
		}
	}

	void tcp_connect_event(){
		{
			PacketMaker::start(0x01);
			PacketMaker::add_string(Nickname);
			PacketMaker::add_string(vm_get_imei()==0?"No Imei present":vm_get_imei());
			PacketMaker::add_string("1.19");
			PacketMaker::add_string(s_ip);
			PacketMaker::add_unsigned_short(s_port);
			PacketMaker::end(TO_TCP);
		}
	}

	void dns_event(){
		Sock::udp_connect(r_udp_port);
	}

};
