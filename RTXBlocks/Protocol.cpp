#include "Protocol.h"
#include "PacketMaker.h"
#include "PacketOpener.h"
#include "PlayerInfo.h"
#include "Player.h"
#include "World.h"
#include "Sock.h"
#include "Time.h"
#include "Chat.h"
#include "Entity.h"
#include "UUID.h"
#include "main.h"
#include <cstdlib>
#include "string.h"
#ifdef MRE
#include "../RTXBlocksVxp/Log.h"
#endif // MRE



extern bool sock_is_conected;
extern unsigned char* tcp_in_buf, * tcp_out_buf;
extern int tcp_in_buf_pos, tcp_out_buf_pos;

extern unsigned char* udp_in_buf, * udp_out_buf;
extern int udp_in_buf_pos, udp_out_buf_pos;

#define TO_TCP tcp_out_buf, tcp_out_buf_pos
#define TO_UDP udp_out_buf, udp_out_buf_pos

//temp
#ifdef GCC
#include <D:\my_ip.txt>
#else
const char* r_ip = "127.0.0.1";
#endif // REAL
unsigned short r_tcp_port = 25501;
unsigned short r_udp_port = 25502;
const char* s_ip = "127.0.0.1";//"MY-PRETTY";
unsigned short s_port = 25565;
const char* Nickname = "RTX";
int my_id = -1;

bool play = 0;

bool pl_nrm = 0;

extern int start_chunk_x;
extern int start_chunk_z;
extern int start_chunk_y;


extern int center_chunk_y;


extern Player_s player;
extern UUID my_uuid;
extern int render_c;

namespace Protocol
{

	void init() {}
	void update() {
		Sock::update();
		open_packets();
	}
	void connect(/*...*/) {
		Sock::tcp_connect(r_ip, r_tcp_port);
		Sock::get_ip_by_dns(r_ip);
	}

	void open_packets() {
		int i = 0;
		for (; i < tcp_in_buf_pos;) {
			int size_size = 0;
			int size = PacketOpener::read_VarInt_by_adress(tcp_in_buf + i, &size_size);
			if (size_size + size + i <= tcp_in_buf_pos) {
				int id = PacketOpener::open(tcp_in_buf + i, size, size_size);
				open_packet(id);

				i += size_size + size;
			}
			else
				break;
		}
		if (i != 0 && tcp_in_buf_pos != i)
			memmove(tcp_in_buf, tcp_in_buf + i, tcp_in_buf_pos - i);
		tcp_in_buf_pos -= i;
	}

	void open_packet(int id) {
		if (!play) {
			switch (id)
			{
			case 0x01:
				//
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
			case 0x02:
			{

				PacketOpener::read_UUID(&my_uuid);
#ifndef MRE
				printf(my_uuid.get_string());
#endif // !MRE

				play = true;

				{
					PacketMaker::start(0x07);
					PacketMaker::add_string("en_GB");
					PacketMaker::add_byte(2);
					PacketMaker::add_VarInt(9);
					PacketMaker::add_unsigned_byte(0x7F);
					PacketMaker::add_VarInt(1);
					PacketMaker::add_bool(0);
					PacketMaker::add_bool(1);
					PacketMaker::end(TO_TCP);
				}
			}
			break;
			}

		}
		else {
			switch (id) {
			case 0x00:
			{
				entity& tmp = *Entity::get_null();

				tmp.id = PacketOpener::read_VarInt();
				PacketOpener::read_UUID(&tmp.uuid);
				tmp.type = PacketOpener::read_VarInt();
				tmp.x = PacketOpener::read_double();
				tmp.y = PacketOpener::read_double();
				tmp.z = PacketOpener::read_double();
				tmp.yaw = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				tmp.pitch = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				tmp.hyaw = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				tmp.data = PacketOpener::read_VarInt();
				tmp.vx = PacketOpener::read_short();
				tmp.vy = PacketOpener::read_short();
				tmp.vz = PacketOpener::read_short();
				tmp.is_player = false;
				Entity::get_make_null_as_entity();
			}
			break;
			case 0x02:
			{
				entity& tmp = *Entity::get_null();

				tmp.id = PacketOpener::read_VarInt();
				PacketOpener::read_UUID(&tmp.uuid);
				tmp.x = PacketOpener::read_double();
				tmp.y = PacketOpener::read_double();
				tmp.z = PacketOpener::read_double();
				tmp.yaw = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				tmp.pitch = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				tmp.is_player = true;
				Entity::get_make_null_as_entity();
			}
			break;
			case 0x63: {
				int eid = PacketOpener::read_VarInt();
				entity* ent_p = Entity::get_find_by_id(eid);
				if (!ent_p)
					break;
				entity& ent = *ent_p;
				ent.x = PacketOpener::read_double();
				ent.y = PacketOpener::read_double();
				ent.z = PacketOpener::read_double();
				ent.yaw = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				ent.pitch = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				ent.is_ground = PacketOpener::read_bool();

			}break;
			case 0x26: {
				int eid = PacketOpener::read_VarInt();
				entity* ent_p = Entity::get_find_by_id(eid);
				if (!ent_p)
					break;
				entity& ent = *ent_p;
				ent.x += double(PacketOpener::read_short()) / 2096.;
				ent.y += double(PacketOpener::read_short()) / 2096.;
				ent.z += double(PacketOpener::read_short()) / 2096.;
			}break;
			case 0x27: {
				int eid = PacketOpener::read_VarInt();
				entity* ent_p = Entity::get_find_by_id(eid);
				if (!ent_p)
					break;
				entity& ent = *ent_p;
				ent.x += double(PacketOpener::read_short()) / 2096.;
				ent.y += double(PacketOpener::read_short()) / 2096.;
				ent.z += double(PacketOpener::read_short()) / 2096.;
				ent.yaw = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				ent.pitch = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				ent.is_ground = PacketOpener::read_bool();
			}break;
			case 0x28: {
				int eid = PacketOpener::read_VarInt();
				entity* ent_p = Entity::get_find_by_id(eid);
				if (!ent_p)
					break;
				entity& ent = *ent_p;
				ent.yaw = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				ent.pitch = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
				ent.is_ground = PacketOpener::read_bool();
			}break;
			case 0x3C: {
				int eid = PacketOpener::read_VarInt();
				entity* ent_p = Entity::get_find_by_id(eid);
				if (!ent_p)
					break;
				entity& ent = *ent_p;
				ent.hyaw = (float)(PacketOpener::read_byte()) / 255.f * 360.f;
			}break;
			case 0x38: {
				int count = PacketOpener::read_VarInt();
				for (int i = 0; i < count; ++i) {
					int eid = PacketOpener::read_VarInt();
					Entity::delete_by_id(eid);
				}
			}break;
			case 0x09:
			{
				unsigned long long val = PacketOpener::read_long();
				int x = val >> 38;
				int y = val & 0xFFF;
				int z = (val >> 12) & 0x3FFFFFF;
				if (x >= (1 << 25)) x -= (1 << 26);
				if (y >= (1 << 11)) y -= (1 << 12);
				if (z >= (1 << 25)) z -= (1 << 26);
				unsigned short id = PacketOpener::read_unsigned_short();
				World::set_block(x, y, z, id);
				//printf("%d %d %d %d\n", x, y, z, id);

				render_c = 1;
			}
			break;
			case 0x30:
			{
				char text[300], who[100];
				PacketOpener::read_string(text);
				int type = PacketOpener::read_VarInt();
				PacketOpener::read_string(who);
				Chat::add_text(who, text);
			}
			break;
			case 0x3d:
			{
				unsigned long long val = PacketOpener::read_long();
				int cx = val >> 42;
				int cy = (val << 44) >> 44;
				int cz = (val << 22) >> 42;
				if (cx >= (1 << 21)) cx -= (1 << 22);
				if (cy >= (1 << 19)) cy -= (1 << 20);
				if (cz >= (1 << 21)) cz -= (1 << 22);
				int count = PacketOpener::read_VarInt();

				//printf("%d %d %d %d\n", cx, cy, cz, count);

				for (int i = 0; i < count; ++i) {
					int pos_and_id = PacketOpener::read_VarInt();
					int x = (pos_and_id >> 8) & 0xF;
					int z = (pos_and_id >> 4) & 0xF;
					int y = pos_and_id & 0xF;
					unsigned short id = pos_and_id >> 12;
					World::set_block(cx * 16 + x, cy * 16 + y, cz * 16 + z, id);
					//printf("\t%d %d %d, %d %d %d, %d\n", x, y, z, cx * 16 + x, cy * 16 + y, cz * 16 + z, id);
				}

				//printf("\n");

				render_c = 1;
			}
			break;
			case 0x34:
			{
				bool new_flag = false;
				int action = PacketOpener::read_VarInt();
				int number_of_player = PacketOpener::read_VarInt();

				for (int i = 0; i < number_of_player; ++i) {
					UUID t_uuid;
					PacketOpener::read_UUID(&t_uuid);

					if (action == 0) {
						player_info* new_pl = PlayerInfo::get_find_by_uuid(&t_uuid);
						if (!new_pl) {
							new_pl = PlayerInfo::get_null();
							new_flag = true;
						}

						new_pl->uuid = t_uuid;

						//char* nick = PacketOpener::read_string();

						//if (strlen(nick) > 16)
						//	break;

						//strcpy(new_pl->nick, nick);
						PacketOpener::read_string(new_pl->nick);

						int number_of_properties = PacketOpener::read_VarInt();

						for (int j = 0; j < number_of_properties; ++j) {
							PacketOpener::read_string();
							PacketOpener::read_string();
							if (PacketOpener::read_bool())
								PacketOpener::read_string();
						}

						new_pl->gamemode = PacketOpener::read_VarInt();
						new_pl->ping = PacketOpener::read_VarInt();

						if (new_flag)
							PlayerInfo::get_make_null_as_pl();
					}
					else if (action == 4) {
						PlayerInfo::delete_by_uuid(&t_uuid);
					}
					else {
						player_info* find_pl = PlayerInfo::get_find_by_uuid(&t_uuid);
						if (!find_pl)
							break;

						if (action == 1)
							find_pl->gamemode = PacketOpener::read_VarInt();
						else if (action == 2)
							find_pl->ping = PacketOpener::read_VarInt();

					}
				}
				//World::set_center_chunk(x, z);
			}
			break;

			case 0x36:
			{
				double x = 0, y = 0, z = 0, yaw = 0, pitch = 0;
				unsigned char flag = 0;
				int tp_ip = 0;
				bool dv = 0;
				x = PacketOpener::read_double();
				y = PacketOpener::read_double();
				z = PacketOpener::read_double();
				yaw = PacketOpener::read_float();
				pitch = PacketOpener::read_float();
				flag = PacketOpener::read_byte();

				World::set_center_chunk(int(x) / 16, int(z) / 16, int(y) / 16);
				pl_nrm = 1;

				if (flag & 0x01)
					player.x += x;
				else
					player.x = x - start_chunk_x * 16;

				if (flag & 0x02)
					player.y += y;
				else
					player.y = y - start_chunk_y * 16;

				if (flag & 0x04)
					player.z += z;
				else
					player.z = z - start_chunk_z * 16;

				if (flag & 0x08)
					player.a += yaw * pi / 180.f;
				else
					player.a = (yaw + 90) * pi / 180.f;

				if (flag & 0x010)
					player.b += pitch * pi / 180.f;
				else
					player.b = (pitch + 90) * pi / 180.f;
				player.acs = 0;
				tp_ip = PacketOpener::read_VarInt();
				dv = PacketOpener::read_bool();
#ifndef MRE
				printf("coreection\n");
#endif // !MRE

				{
					PacketMaker::start(0x00);
					PacketMaker::add_VarInt(tp_ip);
					PacketMaker::end(TO_TCP);
				}

				//p_pos = 1;
				//move = -1;
			}
			break;
			case 0x48:
			{
				int x = PacketOpener::read_VarInt();
				int z = PacketOpener::read_VarInt();
				if (pl_nrm)
					World::set_center_chunk(start_chunk_x + int(player.x) / 16, start_chunk_z + int(player.z) / 16, start_chunk_y + int(player.y) / 16);
				render_c = 1;
				//World::set_center_chunk(x, z, center_chunk_y);
			}
			break;
			case 0x59:
			{
				long long wa = PacketOpener::read_long();
				long long tod = PacketOpener::read_long();
				Time::set_time(wa, tod);
			}
			break;

			case 0x5F:
			{
				char text[1000];
				PacketOpener::read_string(text);
				Chat::add_text(text);
			}
			break;

			case 0x1F:
			{
				int x = PacketOpener::read_VarInt();
				int y = PacketOpener::read_VarInt();
				int z = PacketOpener::read_VarInt();
				World::load_chunck_section(x, y, z, (unsigned short*)(PacketOpener::get_buf() + PacketOpener::get_pos()));
				render_c = 1;
			}
			break;

			}
		}
	}

	void send_message(char* m) {
		if (m[0] == '/') {
			send_command(m + 1);
			return;
		}
		PacketMaker::start(0x04);
		PacketMaker::add_string(m);
		PacketMaker::add_long(Time::get_time_1970());
		PacketMaker::add_long(0);
		PacketMaker::add_VarInt(0);
		PacketMaker::add_bool(0);
		PacketMaker::end(TO_TCP);
	}
	void send_command(char* m) {
		short sp_adr[50] = {};
		int spaces = 0;// , sp_i = 0;
		//for (int i = 0; m[i]; ++i)
		//	if (m[i] == ' ') {
		//		sp_adr[spaces] = i;
		//		++spaces;
		//		m[i] = 0;
		//	}
		PacketMaker::start(0x03);
		PacketMaker::add_string(m);
		PacketMaker::add_long(Time::get_time_1970());
		PacketMaker::add_long(0);//salt
		PacketMaker::add_VarInt(spaces);
		for (int i = 0; i < spaces; ++i) {
			PacketMaker::add_string(m + sp_adr[i] + 1);
			PacketMaker::add_VarInt(0);//len and data
		}
		PacketMaker::add_bool(0);//signed
		PacketMaker::end(TO_TCP);
	}

	void Set_Player_Rotation() {
		PacketMaker::start(0x15);
		PacketMaker::add_float((player.a * 180.f / pi) - 90.f);
		PacketMaker::add_float((player.b * 180.f / pi) - 90.f);
		PacketMaker::add_bool(player.on_ground);
		PacketMaker::end(TO_TCP);
	}

	void Set_Player_Position() {
		PacketMaker::start(0x13);
		PacketMaker::add_double(player.x + start_chunk_x * 16);
		PacketMaker::add_double(player.y + start_chunk_y * 16);
		PacketMaker::add_double(player.z + start_chunk_z * 16);
		PacketMaker::add_bool(player.on_ground);
		PacketMaker::end(TO_TCP);
		World::set_center_chunk(start_chunk_x + int(player.x) / 16, start_chunk_z + int(player.z) / 16, start_chunk_y + int(player.y) / 16);
	}

	void respawn() {
		PacketMaker::start(0x6);
		PacketMaker::add_VarInt(0);
		PacketMaker::end(TO_TCP);
	}

	void tcp_connect_event() {
		{
			PacketMaker::start(0x01);
			PacketMaker::add_string(Nickname);
			PacketMaker::add_string("Run on PC         ");
			PacketMaker::add_string("1.19");
			PacketMaker::add_string(s_ip);
			PacketMaker::add_unsigned_short(s_port);
			PacketMaker::add_VarInt(WORLD__RC_S_X);
			PacketMaker::add_VarInt(WORLD__RC_S_Y);
			PacketMaker::add_VarInt(WORLD__RC_S_Z);
			PacketMaker::end(TO_TCP);
		}
	}

	void dns_event() {
		Sock::udp_connect(r_udp_port);
	}

};
