#pragma once


namespace PacketMaker
{
	void init();

	void start(int packet_id);
	void end(unsigned char* sbuf, int& pos);

	void add_bool(bool v);
	void add_byte(char v);
	void add_unsigned_byte(unsigned char v);
	void add_short(short v);
	void add_unsigned_short(unsigned short v);
	void add_int(int v_);
	void add_long(long long v_);
	void add_float(float v_);
	void add_double(double v_);
	void add_string(const char* v);

	void add_VarInt(int value);

	void add_byte_array(const unsigned char* v, int size);

	void deinit();
};

