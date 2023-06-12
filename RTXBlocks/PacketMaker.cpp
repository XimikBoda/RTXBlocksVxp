#include "PacketMaker.h"
#include "string.h"
#include <corecrt_malloc.h>

static const int SEGMENT_BITS = 0x7F;
static const int CONTINUE_BIT = 0x80;

static const int buf_max_size = 10240;

static unsigned char* buf;
static int buf_pos = 0, cur_id = 0;

namespace PacketMaker
{
	void init() {
		buf = (unsigned char*)malloc(buf_max_size);
	}

	void start(int packet_id) {
		cur_id = packet_id;
		buf_pos = 0;
		add_VarInt(packet_id);
	}
	void end(unsigned char* sbuf, int& pos) {
		int size = buf_pos;
		add_VarInt(size);

		memcpy(&sbuf[pos], &buf[size], buf_pos - size);//size
		pos += buf_pos - size;

		memcpy(&sbuf[pos], &buf[0], size);//data
		pos += size;
	}

	void add_bool(bool v) {
		buf[buf_pos++] = (unsigned char)v;
	}

	void add_byte(char v) {
		buf[buf_pos++] = v;
	}

	void add_unsigned_byte(unsigned char v) {
		buf[buf_pos++] = v;
	}

	void add_short(short v) {
		add_unsigned_short(v);
	}

	void add_unsigned_short(unsigned short v) {
		buf[buf_pos++] = (unsigned char)((v >> 8) & 0xFF);
		buf[buf_pos++] = (unsigned char)(v & 0xFF);
	}

	void add_int(int v_) {
		unsigned int v = v_;
		buf[buf_pos++] = (unsigned char)((v >> 24) & 0xFF);
		buf[buf_pos++] = (unsigned char)((v >> 16) & 0xFF);
		buf[buf_pos++] = (unsigned char)((v >> 8) & 0xFF);
		buf[buf_pos++] = (unsigned char)(v & 0xFF);
	}

	void add_long(long long v_) {
		unsigned int v = ((unsigned long long)(v_) >> 32);
		buf[buf_pos++] = (unsigned char)((v >> 24) & 0xFF);
		buf[buf_pos++] = (unsigned char)((v >> 16) & 0xFF);
		buf[buf_pos++] = (unsigned char)((v >> 8) & 0xFF);
		buf[buf_pos++] = (unsigned char)(v & 0xFF);
		v = (unsigned int)v_;
		buf[buf_pos++] = (unsigned char)((v >> 24) & 0xFF);
		buf[buf_pos++] = (unsigned char)((v >> 16) & 0xFF);
		buf[buf_pos++] = (unsigned char)((v >> 8) & 0xFF);
		buf[buf_pos++] = (unsigned char)(v & 0xFF);
	}
	void add_float(float v_) {
		unsigned int v = *(unsigned int*)(&v_);
		buf[buf_pos++] = (unsigned char)((v >> 24) & 0xFF);
		buf[buf_pos++] = (unsigned char)((v >> 16) & 0xFF);
		buf[buf_pos++] = (unsigned char)((v >> 8) & 0xFF);
		buf[buf_pos++] = (unsigned char)(v & 0xFF);
	}
	void add_double(double v_) {
		add_long(*(unsigned long long*)(&v_));
	}

	void add_string(const char* v) {
		int size = strlen(v);
		add_VarInt(size);
		add_byte_array((const unsigned char*)v, size);
	}

	/*void add_string_UTF8(const char* v){
		unsigned char dest[32767*4];
		vm_chset_convert(VM_CHSET_ASCII, VM_CHSET_UTF8, (VMCHAR*)v, (VMCHAR*)dest, (strlen(v)+1)*4);
		int size = strlen((char*)dest);
		add_VarInt(size);
		add_byte_array(dest, size);
	}*/

	void add_VarInt(int value_) {
		unsigned int value = value_;
		while (true) {
			if ((value & ~SEGMENT_BITS) == 0) {
				buf[buf_pos++] = (unsigned char)(value & 0xFF);;
				return;
			}

			buf[buf_pos++] = (unsigned char)(((value & SEGMENT_BITS) | CONTINUE_BIT) & 0xFF);;

			// Note: >>> means that the sign bit is shifted with the rest of the number rather than being left alone
			value >>= 7;
		}
	}

	void add_byte_array(const unsigned char* v, int size) {
		memcpy(&buf[buf_pos], v, size);
		buf_pos += size;
	}

	void deinit() {
		free((void*)buf);
	}
};

