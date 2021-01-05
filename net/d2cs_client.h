#ifndef INCLUDED_D2CS_CLIENT_H
#define INCLUDED_D2CS_CLIENT_H

#include "bn_types.h"

#pragma pack(push, 1)
typedef struct
{
	bn_short	size;
	bn_short	type;
	bn_int		seqno;			/* seqno, set by the sender */
} t_d2cs_d2gs_header;

typedef struct
{
	t_d2cs_d2gs_header	h;
} t_d2cs_d2gs_generic;

#define D2CS_D2GS_AUTHREQ		0x10
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			sessionnum;
	bn_int			signlen;
	/* realm name */
	/* key check sum, maybe 128 bytes */
} t_d2cs_d2gs_authreq;

#define D2GS_D2CS_AUTHREPLY		0x11
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			version;
	bn_int			checksum;
	bn_int			randnum;
	bn_int			signlen;
	bn_basic		sign[128];
} t_d2gs_d2cs_authreply;

#define D2CS_D2GS_AUTHREPLY		0x11
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			reply;
} t_d2cs_d2gs_authreply;
#define D2CS_D2GS_AUTHREPLY_SUCCEED			0x00
#define D2CS_D2GS_AUTHREPLY_BAD_VERSION		0x01
#define D2CS_D2GS_AUTHREPLY_BAD_CHECKSUM	0x02

#define D2GS_D2CS_SETGSINFO		0x12
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			maxgame;
	bn_int			gameflag;
} t_d2gs_d2cs_setgsinfo;

#define D2CS_D2GS_ECHOREQ		0x13
typedef struct {
	t_d2cs_d2gs_header	h;
} t_d2cs_d2gs_echoreq;


#define D2GS_D2CS_GETCONF		0x15
typedef struct
{
	t_d2cs_d2gs_header	h;
} t_d2gs_d2cs_getconf;

#define D2GS_D2CS_ECHOREPLY	0x13
typedef struct {
	t_d2cs_d2gs_header	h;
} t_d2gs_d2cs_echoreply;

#define D2CS_D2GS_CONTROL		0x14
typedef struct
{
	t_d2cs_d2gs_header      h;
	bn_int                  cmd;
	bn_int                  value;
} t_d2cs_d2gs_control;
#define D2CS_D2GS_CONTROL_CMD_RESTART	0x01
#define D2CS_D2GS_CONTROL_CMD_SHUTDOWN	0x02
#define D2CS_D2GS_CONTROL_VALUE_DEFAULT	0x00

#define D2CS_D2GS_SETINITINFO		0x15
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			time;
	bn_int			gs_id;
	bn_int			ac_version;
	/* ac_checksum */
} t_d2cs_d2gs_setinitinfo;

#define D2CS_D2GS_SETCONFFILE		0x16
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			size;
	bn_int			reserved1;
	/* conf file (null terminated string) */
} t_d2cs_d2gs_setconffile;

#define D2CS_D2GS_CREATEGAMEREQ		0x20
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_byte			ladder;
	bn_byte			expansion;
	bn_byte			difficulty;
	bn_byte			hardcore;
	/* gamename */
	/* gamepass */
	/* gamedesc */
	/* create by acctname */
	/* create by charname */
	/* create by ip address(string) */
} t_d2cs_d2gs_creategamereq;

#define D2GAME_DIFFICULTY_NORMAL	0
#define D2GAME_DIFFICULTY_NIGHTMARE	1
#define D2GAME_DIFFICULTY_HELL		2

#define D2GS_D2CS_CREATEGAMEREPLY	0x20
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			result;
	bn_int			gameid;
} t_d2gs_d2cs_creategamereply;
#define D2GS_D2CS_CREATEGAME_SUCCEED	0
#define D2GS_D2CS_CREATEGAME_FAILED		1


#define D2CS_D2GS_JOINGAMEREQ		0x21
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			gameid;
	bn_int			token;
	bn_int			vip_expire;
	/* character name */
	/* account name */
	/* client ip address(string) */
} t_d2cs_d2gs_joingamereq;

#define D2GS_D2CS_JOINGAMEREPLY		0x21
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			result;
	bn_int			gameid;
} t_d2gs_d2cs_joingamereply;
#define D2GS_D2CS_JOINGAME_SUCCEED			0
#define D2GS_D2CS_JOINGAME_FAILED			1

#define D2GS_D2CS_UPDATEGAMEINFO	0x22
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			flag;
	bn_int			gameid;
	bn_int			charlevel;
	bn_int			charclass;
	bn_int			addr;
	/* charname */
} t_d2gs_d2cs_updategameinfo;
#define D2GS_D2CS_UPDATEGAMEINFO_FLAG_UPDATE	0
#define D2GS_D2CS_UPDATEGAMEINFO_FLAG_ENTER		1
#define D2GS_D2CS_UPDATEGAMEINFO_FLAG_LEAVE		2

#define D2GS_D2CS_CLOSEGAME		0x23
typedef struct
{
	t_d2cs_d2gs_header	h;
	bn_int			gameid;
} t_d2gs_d2cs_closegame;

#pragma pack(pop)

#include "d2xs_client.hpp"

#include <future>

namespace Net {
	class D2CSClient {
	public:
		D2CSClient(const D2CSClient&) = delete;
		D2CSClient(asio::io_context& io_context,
			const std::string& server_host, const int server_port)
			: net_client_("D2CSClient", io_context, server_host, server_port) {
			connected_ = false;
			net_client_.OnError([&](std::error_code ec) {
				connected_ = false;
			});
			net_client_.Connect([&]() {
				connected_ = true;
			});
		}

		bool connected() const {
			return connected_;
		}

		void SetGSInfoAsync(int max_game, int game_flag);
		void UpdateGameInfoAsync(int flag, int game_id, int char_level, int char_class, int addr);
		void CloseGameAsync(int game_id);

		std::future<std::string> GetConfAsync();

	private:
		D2XSClient<t_d2cs_d2gs_header> net_client_;
		bool connected_;
	};
}

#endif