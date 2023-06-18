#include "PacketOpener.h"
#include "string.h"
#include "miniz.h"
#include <cstdlib>
#ifndef MRE
#include <corecrt_malloc.h>
#endif // !MRE

static const int SEGMENT_BITS = 0x7F;
static const int CONTINUE_BIT = 0x80;

static const int buf_max_size = 10240;

static unsigned char* buf;
static int buf_pos = 0, cur_id = 0;

namespace PacketOpener
{
	void init() {
		buf = (unsigned char*)malloc(buf_max_size);
	}

	unsigned char* get_buf() {
		return buf;
	}

	int& get_pos() {
		return buf_pos;
	}

	int open(unsigned char* a, int size, int size_size) { //load
		buf_pos = 0;
		/*if(compres){
			int size_unsize = 0;
			int uncompress_size = read_VarInt_by_adress(a+size_size, &size_unsize), ucs2 = uncompress_size;
			if(uncompress_size == 0){
				int size_id = 0;
				cur_g_id = read_VarInt_by_adress(a+size_size+size_unsize, &size_id);
				memcpy(buf_g, a + size_size + size_unsize + size_id, size);
			}else{
				int tst = uncompress(buf_g, (uLongf*)&ucs2, a + size_size + size_unsize, size - size_unsize);
				if(Z_OK!=tst)
					show_error_and_exit("Uncompress error");
				int size_id = 0;
				cur_g_id = read_VarInt_by_adress(buf_g, &size_id);
				buf_g_pos = size_id;
			}


		}else{*/
		int size_id = 0;
		cur_id = read_VarInt_by_adress(a + size_size, &size_id);
		if (cur_id == 0x1F) {
			mz_ulong dest_len= buf_max_size;
			if (mz_uncompress(buf, &dest_len, a + size_size + size_id, size - size_id))
				printf("wrong");
		}
		else
			memcpy(buf, a + size_size + size_id, size - size_id);
		//}
		return cur_id;
	}

	bool read_bool() {
		return buf[buf_pos++];
	}

	char read_byte() {
		return buf[buf_pos++];
	}

	unsigned char read_unsigned_byte() {
		return buf[buf_pos++];
	}

	short read_short() {
		return read_unsigned_short();
	}

	unsigned short read_unsigned_short() {
		unsigned short v =
			(read_unsigned_byte() << 8) |
			(read_unsigned_byte());
		return v;
	}


	int read_int() {
		unsigned int v =
			(read_unsigned_byte() << 24) |
			(read_unsigned_byte() << 16) |
			(read_unsigned_byte() << 8) |
			(read_unsigned_byte());
		return v;
	}

	unsigned int read_unsigned_int() {
		unsigned int v =
			(read_unsigned_byte() << 24) |
			(read_unsigned_byte() << 16) |
			(read_unsigned_byte() << 8) |
			(read_unsigned_byte());
		return v;
	}

	long long read_long() {
		unsigned int v1 = read_int(), v2 = read_int();
		/*unsigned long long v =
			(read_unsigned_byte()<<56) |
			(read_unsigned_byte()<<48) |
			(read_unsigned_byte()<<40) |
			(read_unsigned_byte()<<32) |
			(read_unsigned_byte()<<24) |
			(read_unsigned_byte()<<16) |
			(read_unsigned_byte()<<8) |
			(read_unsigned_byte());*/
		return (((unsigned long long)v1) << 32) | ((unsigned long long)v2);
	}

	float read_float() {
		int v = read_int();
		return *(float*)(&v);
	}

	double read_double() {
		unsigned long long v = read_long();
		return *(double*)(&v);
	}

	int read_VarInt() {
		int value = 0;
		int position = 0;
		unsigned char currentByte;

		while (true) {
			currentByte = buf[buf_pos++];
			value |= (currentByte & SEGMENT_BITS) << position;

			if ((currentByte & CONTINUE_BIT) == 0) break;

			position += 7;

			//if (position >= 32)
			//	show_error_and_exit("VarInt is too big");
		}

		return value;
	}
	void NBT_list() {
		int tag_t = read_byte();
		int size = read_int();
		if (size < 0)
			size = 0;
		buf_pos += read_int();
	}
	void recursive_NBT(bool a = 0) {
		int tag_t = 0;
		while (tag_t = read_byte()) {
			unsigned short str_len = read_short();
			//printf("\n%s\n", &buf[buf_pos]);
			switch (tag_t) {
			case 1:
				buf_pos += 1 + str_len;
				break;
			case 2:
				buf_pos += 2 + str_len;
				break;
			case 3:
				buf_pos += 4 + str_len;
				break;
			case 4:
				buf_pos += 8 + str_len;
				break;
			case 5:
				buf_pos += 4 + str_len;
				break;
			case 6:
				buf_pos += 8 + str_len;
				break;
			case 7:
				buf_pos += str_len;
				buf_pos + read_int();
				break;
			case 8:
				buf_pos += str_len;
				buf_pos += read_short();
				break;
			case 9:
				buf_pos += str_len;
				NBT_list();
				break;
			case 10:
				buf_pos += str_len;
				recursive_NBT(1);
				break;
			case 11:
				buf_pos += str_len;
				buf_pos += read_int() * 4;
				break;
			case 12:
				buf_pos += str_len;
				buf_pos += read_int() * 8;
				break;
			default:
				if (a)
					1;//printf("err");
				else
					--buf_pos;
				break;
			}
		}
	}

	void skip_NBT() {
		recursive_NBT();
	}

	bool read_Heightmaps_NBT(unsigned short* a) {
		if (read_byte() == 10 && read_short() == 0) {
			int tag_t = 0;
			while (tag_t = read_byte()) {
				unsigned short str_len = read_short();
				if (tag_t != 12)
					return 0;
				if (read_byte() == 'M') {
					buf_pos += str_len - 1;
					int n = read_int();
					unsigned int tmp = 0, tmp_i = 0;
					int a_i = 0;
					for (int i = 0; i < n; ++i) {
						unsigned long long v = read_long();
						for (int t = 0; t < 64 - 9 && a_i < 256; t += 9)
							a[a_i++] = (v >> (t)) & 0x1FF;


					}
				}
				else {
					buf_pos += str_len - 1;
					int t = read_int();
					buf_pos += t * 8;
				}


			}
		}
		else
			return 0;
		return 1;
	}

	void read_UUID(UUID *u) {
		u->a1 = read_unsigned_int();
		u->a2 = read_unsigned_int();
		u->a3 = read_unsigned_int();
		u->a4 = read_unsigned_int();

	}

	char* read_string() {
		int size = read_VarInt();

		char* adr = (char*)&buf[buf_pos];

		buf_pos += size;

		return adr;
	}

	void read_string(char * p) {
		int size = read_VarInt();

		char* adr = (char*)&buf[buf_pos];
		memcpy(p, adr, size);

		p[size] = 0;

		buf_pos += size;

		//return adr;
	}

	int get_len_to_end() {
		return buf_max_size - buf_pos;
	}

	int read_VarInt_by_adress(unsigned char* a, int* size) {
		int value = 0;
		int position = 0;
		unsigned char currentByte;

		while (true) {
			currentByte = *(a++);
			if (size)
				++(*size);
			value |= (currentByte & SEGMENT_BITS) << position;

			if ((currentByte & CONTINUE_BIT) == 0) break;

			position += 7;

			//if (position >= 32)
			//	show_error_and_exit("VarInt is too big");
		}

		return value;
	}

	void deinit() {
		free((void*)buf);
	}
};

