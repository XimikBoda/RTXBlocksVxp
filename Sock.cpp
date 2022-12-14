#include "Sock.h"
#include "main.h"
#include "vmsock.h"

unsigned char *tcp_in_buf=0,  *tcp_out_buf=0;
int tcp_in_buf_pos=0, tcp_out_buf_pos=0;

unsigned char *udp_in_buf=0,  *udp_out_buf=0;
int udp_in_buf_pos=0, udp_out_buf_pos=0;

static int tcp_handle = -1;
static int udp_handle = -1;

static vm_sockaddr_struct udp_addr;

namespace Sock
{
	void init(){
		udp_addr.addr_len = 0;
		tcp_in_buf =  (unsigned char*)vm_malloc(tcp_in_buf_size);
		tcp_out_buf = (unsigned char*)vm_malloc(tcp_out_buf_size);
		udp_in_buf =  (unsigned char*)vm_malloc(udp_in_buf_size);
		udp_out_buf = (unsigned char*)vm_malloc(udp_out_buf_size);

		if(tcp_in_buf==0||tcp_out_buf==0||udp_in_buf==0||udp_out_buf==0)
			show_error_and_exit("Sock malloc error");
	}
	//// TCP
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
		if(tcp_handle>=0) {show_error_and_exit("Tcp already opened"); return;}

		tcp_handle = vm_tcp_connect(host, port, 1, tcp_callback);

		if(tcp_handle<0) show_error_and_exit("Tcp open error");
	}

	void tcp_update(){
		if(tcp_handle>=0){
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
	}

	////  UDP
	void udp_callback(VMINT hdl, VMINT event){
		switch(event){
			case VM_UDP_EVT_READ:
				break;
			case VM_UDP_EVT_PIPE_BROKEN:
				show_error_and_exit("Udp pipe broken");
				break;
			case VM_UDP_EVT_PIPE_CLOSED:
				show_error_and_exit("Udp pipe closed");
				break;
		}
	}

	void udp_connect(const VMINT port){
		if(udp_handle>=0) {show_error_and_exit("Udp already opened"); return;}

		if(udp_addr.addr_len==0) {show_error_and_exit("Udp address error"); return;}

		udp_addr.port = port;
		udp_handle = vm_udp_create(rand()%65000, 1, udp_callback, 0);

		if(udp_handle<0) show_error_and_exit("Udp open error");
	}

	void udp_update(){
		vm_sockaddr_struct tmp_addr;
		if(udp_handle>=0){
			int rec = vm_udp_recvfrom(udp_handle, udp_in_buf + udp_in_buf_pos, udp_in_buf_size - udp_in_buf_pos-100, &tmp_addr);
			if(rec<0)
				show_error_and_exit("Udp reed error");
			if(rec>0)
				udp_in_buf_pos+=rec;

			if(udp_out_buf_pos>0){
				int snd = vm_udp_sendto(udp_handle, udp_out_buf, udp_out_buf_pos, &udp_addr);
				if(snd<0)
					show_error_and_exit("Udp write error");
				if(snd>0){
					if(snd!=udp_out_buf_pos)
						memmove(udp_out_buf, udp_out_buf+snd, udp_out_buf_pos);
					udp_out_buf_pos-=snd;
				}
			}
		}
	}
	//// DNS
	int d_callback(vm_soc_dns_result*d){
		udp_addr.addr_len = d->num*4;
		(*(unsigned int*)udp_addr.addr)=d->address[0];
		return 0;
	}

	void get_ip_by_dns(const char* host){
		vm_soc_dns_result dns;

		int u = vm_soc_get_host_by_name(1, host, &dns, d_callback);

		if(u==VM_E_SOC_SUCCESS){
			udp_addr.addr_len = dns.num*4;
			(*(unsigned int*)udp_addr.addr)=dns.address[0];
		}else if(u != VM_E_SOC_WOULDBLOCK)
			show_error_and_exit("DNS error");
	}
	/// Update all
	void update(){
		tcp_update();
		udp_update();
	}

	/// Deinit 
	void deinit(){
		if(tcp_handle>=0){
			vm_tcp_close(tcp_handle);
			tcp_handle = -1;
		}
		if(udp_handle>=0){
			vm_udp_close(udp_handle);
			udp_handle = -1;
		}
		vm_free((void*)tcp_in_buf);
		vm_free((void*)tcp_out_buf);
		vm_free((void*)udp_in_buf);
		vm_free((void*)udp_out_buf);
		tcp_in_buf = tcp_out_buf = udp_in_buf = udp_out_buf = 0;
	}
};
