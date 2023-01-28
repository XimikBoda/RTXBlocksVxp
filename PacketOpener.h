#pragma once
namespace PacketOpener
{
	void init();

	int open(unsigned char*a, int size, int size_size);

	bool read_bool();
	char read_byte();
	unsigned char read_unsigned_byte();
	unsigned short read_unsigned_short();
	short read_short();
	int read_int();
	long long read_long();
	float read_float();
	double read_double();
	int read_VarInt();

	void skip_NBT();
	bool read_Heightmaps_NBT(unsigned short *a);

	int read_VarInt_by_adress(unsigned char*a, int *size);

	void deinit();
};

