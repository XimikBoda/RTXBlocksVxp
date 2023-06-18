#include "UUID.h"

static char UUID_temp_str[37];

const char HEX_l[17] = "0123456789abcdef";

const char* UUID::get_string() {
	unsigned char* p = (unsigned char*)&a1;

	char* ch_p = UUID_temp_str;

	for (int i = 0; i < 4; ++i)
		*(ch_p++) = HEX_l[*p >> 4],
		* (ch_p++) = HEX_l[*(p++) & 0xF];

	*(ch_p++) ='-';

	for (int j = 0; j < 3; ++j) {
		for (int i = 0; i < 2; ++i)
			*(ch_p++) = HEX_l[*p >> 4],
			* (ch_p++) = HEX_l[*(p++) & 0xF];

		*(ch_p++) = '-';
	}

	for (int i = 0; i < 6; ++i)
		*(ch_p++) = HEX_l[*p >> 4],
		* (ch_p++) = HEX_l[*(p++) & 0xF];

	*(ch_p++) = 0;

	return UUID_temp_str;
}
