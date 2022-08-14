#pragma once

const int tcp_in_buf_size = 10240*2, tcp_out_buf_size = 10240*2;
const int upd_in_buf_size = 10240*2, udp_out_buf_size = 10240*2;

namespace Sock
{
	void init();
};

