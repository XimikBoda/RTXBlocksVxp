#include "Sock.h"
#include "main.h"
#include "vmsock.h"

unsigned char *tcp_in_buf=0,  *tcp_out_buf=0;
int tcp_in_buf_pos=0, tcp_out_buf_pos=0;

unsigned char *udp_in_buf=0,  *udp_out_buf=0;
int udp_in_buf_pos=0, udp_out_buf_pos=0;

static int tcp_handle = -1;
static int udp_handle = -1;

namespace Sock
{
	void init(){
		tcp_in_buf =  (unsigned char*)vm_malloc(tcp_in_buf_size);
		tcp_out_buf = (unsigned char*)vm_malloc(tcp_out_buf_size);
		udp_in_buf =  (unsigned char*)vm_malloc(upd_in_buf_size);
		udp_out_buf = (unsigned char*)vm_malloc(udp_out_buf_size);

		if(tcp_in_buf==0||tcp_out_buf==0||udp_in_buf==0||udp_out_buf==0)
			show_error_and_exit("Sock malloc error");
	}

	void tcp_callback(VMINT handle, VMINT event){
		switch(event){
			case VM_TCP_EVT_CONNECTED:
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
	}

	void tcp_connect(const char* host, const VMINT port){
		tcp_handle = vm_tcp_connect(host, port, 1, tcp_callback);
		if(tcp_handle<=0)
			show_error_and_exit("Tcp open error");
	}

	void update(){
		int rec = vm_tcp_read(tcp_handle, tcp_in_buf + tcp_in_buf_pos, tcp_in_buf_size - tcp_in_buf_pos-100);
		if(rec<0)
			show_error_and_exit("Tcp reed error");
		if(rec>0)
			tcp_in_buf_pos+=rec;

		if(tcp_out_buf_pos>0){
			int snd = vm_tcp_write(tcp_handle, tcp_out_buf, tcp_out_buf_pos);
			if(snd<0)
				show_error_and_exit("Tcp write error");
			if(snd>0){
				if(snd!=tcp_out_buf_pos)
					memmove(tcp_out_buf, tcp_out_buf+snd, tcp_out_buf_pos);
				tcp_out_buf_pos-=snd;
			}
		}
	}
};
