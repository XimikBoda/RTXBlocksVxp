#include "UIConfigMenu.h"
#include "Render.h"
#include "Keyboard.h"
#include "Protocol.h"
#include "string.h"
#include "UIEngine.h"

#include "cJSON.h"

extern UIEngine uiengine;

int read_from_file_to_addr(const char* path, void** addr);
void write_from_addr_to_file(const char* path, void* addr, int size);

const char* settings_filename = "settings.json";

extern char r_ip[100];
extern unsigned short r_tcp_port;
extern unsigned short r_udp_port;
extern char s_ip[100];
extern unsigned short s_port;
extern char Nickname[17];

extern GameState gameState;

UIConfigMenu::UIConfigMenu()
{
	if (uiarray[cur_element])
		uiarray[cur_element]->isFocused = 1;

	LoadSettings();
}

void UIConfigMenu::Draw(unsigned short* buf)
{
	Render::draw_fill_rect(buf, 0, 0, s_w, s_h, 0x0000);

	int y = 3;
	int x = 5;

	Render::draw_text_white_centered(buf, 0, y, s_w, "RTXBlocks");

	Render::draw_fill_rect(buf, 0, y + 13, s_w, 1, 0xFFFF);

	y += 20;
	Render::draw_text_white(buf, 5, y, "Nickname:");
	y += 15;

	nicknameInput.y = y;

	y += 20;
	Render::draw_text_white(buf, 5, y, "Minecraft server:");
	y += 15;

	serverSelecter.y = y;

	y += 17;

	for (int i = 0, x = 5; i < 5; ++i, x += 5 + 14) {
		UIBase* el = uiarray[2 + i];
		el->y = y;
		el->x = x;
	}

	y += 20;
	Render::draw_text_white(buf, 5, y, "Relay server:");
	y += 15;

	relayServerSelecter.y = y;

	y += 17;

	for (int i = 0, x = 5; i < 5; ++i, x += 5 + 14) {
		UIBase* el = uiarray[8 + i];
		el->y = y;
		el->x = x;
	}

	y += 20;
	startButton.y = y;

	for (int i = 0; i < sizeof(uiarray) / sizeof(uiarray[0]); ++i)
		uiarray[i]->QDraw(buf);
}

void UIConfigMenu::KeyboardEvent(int event, int keycode)
{
	switch (event) {
	case VM_KEY_EVENT_UP:
		switch (keycode) {
		case VM_KEY_UP:
			MoveFocus(uiarray_moves[cur_element][0]);
			break;
		case VM_KEY_DOWN:
			MoveFocus(uiarray_moves[cur_element][1]);
			break;
		case VM_KEY_LEFT:
			MoveFocus(uiarray_moves[cur_element][2]);
			break;
		case VM_KEY_RIGHT:
			MoveFocus(uiarray_moves[cur_element][3]);
			break;

		case VM_KEY_OK:
			if (uiarray[cur_element])
				uiarray[cur_element]->OKClick();
			break;
		}
		break;
	}
}

int& UIConfigMenu::ssCurId()
{
	return serverSelecter.cur_id;
}

int UIConfigMenu::ssGetLen()
{
	return minecraftServers.size();
}

const char* UIConfigMenu::ssGetEl(int id)
{
	return minecraftServers[id].get_name();
}

void UIConfigMenu::ssAdd()
{
	minecraftServers.push_back({ "(new)" });
	ssCurId() = ssGetLen() - 1;
	ssEdit();
}

void UIConfigMenu::ssRemove()
{
	if (ssGetLen())
		minecraftServers.erase(minecraftServers.begin() + ssCurId());
}

void UIConfigMenu::ssMoveUp()
{
	if (ssCurId() > 0) {
		std::swap(minecraftServers[ssCurId()], minecraftServers[ssCurId() - 1]);
		ssCurId()--;
	}
}

void UIConfigMenu::ssEdit()
{
	uiServerEdit.start(&(minecraftServers[ssCurId()]));
}

void UIConfigMenu::ssMoveDown()
{
	if (ssCurId() < ssGetLen() - 1) {
		std::swap(minecraftServers[ssCurId()], minecraftServers[ssCurId() + 1]);
		ssCurId()++;
	}
}

int& UIConfigMenu::rssCurId()
{
	return relayServerSelecter.cur_id;
}

int UIConfigMenu::rssGetLen()
{
	return minecraftRServers.size();
}

const char* UIConfigMenu::rssGetEl(int id)
{
	return minecraftRServers[id].get_name();
}

void UIConfigMenu::rssAdd()
{
	minecraftRServers.push_back({ "(new)" });
	rssCurId() = rssGetLen() - 1;
	rssEdit();
}

void UIConfigMenu::rssRemove()
{
	if (rssGetLen())
		minecraftRServers.erase(minecraftRServers.begin() + rssCurId());
}

void UIConfigMenu::rssMoveUp()
{
	if (rssCurId() > 0) {
		std::swap(minecraftRServers[rssCurId()], minecraftRServers[rssCurId() - 1]);
		rssCurId()--;
	}
}

void UIConfigMenu::rssEdit()
{
	uiRServerEdit.start(&(minecraftRServers[rssCurId()]));
}

void UIConfigMenu::rssMoveDown()
{
	if (rssCurId() < rssGetLen() - 1) {
		std::swap(minecraftRServers[rssCurId()], minecraftRServers[rssCurId() + 1]);
		rssCurId()++;
	}
}

void UIConfigMenu::ClickStart()
{
	if (rssCurId() >= 0 && rssCurId() < rssGetLen() &&
		ssCurId() >= 0 && ssCurId() < ssGetLen()) {
		SaveSettings();

		auto &rss = minecraftRServers[rssCurId()];
		if (rss.host.size() >= 100)
			return;
		strcpy(r_ip, rss.host.c_str());
		r_tcp_port = rss.Tport;
		r_udp_port = rss.Uport;

		auto& ss = minecraftServers[ssCurId()];
		if (ss.host.size() >= 100)
			return;
		strcpy(s_ip, ss.host.c_str());
		s_port = ss.port;

		gameState = PlayS;

		uiengine.PopUI();
		Protocol::connect();
	}
}

void UIConfigMenu::SaveSettings()
{
	cJSON* settings = cJSON_CreateObject();
	if (settings == NULL)
		return;

	if (cJSON_AddStringToObject(settings, "Nickname", Nickname) == NULL) {
		cJSON_Delete(settings);
		return;
	}

	{
		cJSON* relays = cJSON_AddArrayToObject(settings, "Relays");
		if (relays == NULL) {
			cJSON_Delete(settings);
			return;
		}

		for (int i = 0; i < minecraftRServers.size(); ++i) {
			cJSON* relay = cJSON_CreateObject();

			if (cJSON_AddStringToObject(relay, "host", minecraftRServers[i].host.c_str()) == NULL) {
				cJSON_Delete(settings);
				return;
			}

			if (cJSON_AddNumberToObject(relay, "tcp port", minecraftRServers[i].Tport) == NULL) {
				cJSON_Delete(settings);
				return;
			}

			if (cJSON_AddNumberToObject(relay, "udp port", minecraftRServers[i].Uport) == NULL) {
				cJSON_Delete(settings);
				return;
			}

			cJSON_AddItemToArray(relays, relay);
		}
	}

	{
		cJSON* servers = cJSON_AddArrayToObject(settings, "Servers");
		if (servers == NULL) {
			cJSON_Delete(settings);
			return;
		}

		for (int i = 0; i < minecraftServers.size(); ++i) {
			cJSON* server = cJSON_CreateObject();

			if (cJSON_AddStringToObject(server, "name", minecraftServers[i].name.c_str()) == NULL) {
				cJSON_Delete(settings);
				return;
			}

			if (cJSON_AddStringToObject(server, "host", minecraftServers[i].host.c_str()) == NULL) {
				cJSON_Delete(settings);
				return;
			}

			if (cJSON_AddNumberToObject(server, "port", minecraftServers[i].port) == NULL) {
				cJSON_Delete(settings);
				return;
			}

			cJSON_AddItemToArray(servers, server);
		}
	}

	if (cJSON_AddNumberToObject(settings, "Last relay", relayServerSelecter.cur_id) == NULL) {
		cJSON_Delete(settings);
		return;
	}

	if (cJSON_AddNumberToObject(settings, "Last server", serverSelecter.cur_id) == NULL) {
		cJSON_Delete(settings);
		return;
	}

	char* out_json = cJSON_Print(settings);

	if (out_json != NULL) {
		write_from_addr_to_file(settings_filename, out_json, strlen(out_json));

		free(out_json);
	}

	cJSON_Delete(settings);
}

void UIConfigMenu::LoadSettings()
{
	char* in_json;
	int in_json_size = read_from_file_to_addr(settings_filename, (void**)&in_json);

	if (in_json == 0) {
		//TODO (default)
		return;
	}

	cJSON* settings = cJSON_ParseWithLength(in_json, in_json_size);

	{
		cJSON* nick = cJSON_GetObjectItemCaseSensitive(settings, "Nickname");
		if (cJSON_IsString(nick) && (nick->valuestring != NULL) && strlen(nick->valuestring) < 17)
			strcpy(Nickname, nick->valuestring);
	}

	{
		std::vector<MinecraftRServerEl> minecraftRServers_tmp;

		cJSON* relay = 0, * relays = cJSON_GetObjectItemCaseSensitive(settings, "Relays");
		cJSON_ArrayForEach(relay, relays) {
			MinecraftRServerEl el;

			cJSON* host = cJSON_GetObjectItemCaseSensitive(relay, "host");
			cJSON* tport = cJSON_GetObjectItemCaseSensitive(relay, "tcp port");
			cJSON* uport = cJSON_GetObjectItemCaseSensitive(relay, "udp port");

			if (cJSON_IsString(host) && (host->valuestring != NULL))
				el.host = host->valuestring;
			else
				return;

			if (cJSON_IsNumber(tport) && tport->valueint >= 0 && tport->valueint <= ((1 << 16) - 1))
				el.Tport = tport->valueint;
			else
				return;

			if (cJSON_IsNumber(uport) && uport->valueint >= 0 && uport->valueint <= ((1 << 16) - 1))
				el.Uport = uport->valueint;
			else
				return;

			minecraftRServers_tmp.push_back(el);
		}

		minecraftRServers = minecraftRServers_tmp;
	}

	{
		std::vector<MinecraftServerEl> minecraftServers_tmp;

		cJSON* server = 0, * servers = cJSON_GetObjectItemCaseSensitive(settings, "Servers");
		cJSON_ArrayForEach(server, servers) {
			MinecraftServerEl el;

			cJSON* name = cJSON_GetObjectItemCaseSensitive(server, "name");
			cJSON* host = cJSON_GetObjectItemCaseSensitive(server, "host");
			cJSON* port = cJSON_GetObjectItemCaseSensitive(server, "port");

			if (cJSON_IsString(name) && (name->valuestring != NULL))
				el.name = name->valuestring;
			else
				return;

			if (cJSON_IsString(host) && (host->valuestring != NULL))
				el.host = host->valuestring;
			else
				return;

			if (cJSON_IsNumber(port) && port->valueint >= 0 && port->valueint <= ((1 << 16) - 1))
				el.port = port->valueint;
			else
				return;

			minecraftServers_tmp.push_back(el);
		}

		minecraftServers = minecraftServers_tmp;
	}

	{
		cJSON* lrelay = cJSON_GetObjectItemCaseSensitive(settings, "Last relay");
		if (cJSON_IsNumber(lrelay))
			relayServerSelecter.cur_id = lrelay->valueint;
	}

	{
		cJSON* lserver = cJSON_GetObjectItemCaseSensitive(settings, "Last server");
		if (cJSON_IsNumber(lserver))
			serverSelecter.cur_id = lserver->valueint;
	}

	cJSON_Delete(settings);
	free(in_json);
}

void UIConfigMenu::MoveFocus(int new_id)
{
	if (new_id > sizeof(uiarray) / sizeof(uiarray[0]) || cur_element == new_id)
		return;

	if (uiarray[cur_element])
		uiarray[cur_element]->isFocused = 0;

	cur_element = new_id;

	if (uiarray[cur_element])
		uiarray[cur_element]->isFocused = 1;
}

