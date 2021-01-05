#ifndef INCLUDED_CONNECTION_H
#define INCLUDED_CONNECTION_H

#include "bn_types.h"

/* for identifying by bnetd server, send just after connected */
typedef struct
{
	bn_byte		bnclass;
} t_d2gs_connect;
#define CONNECT_CLASS_D2GS_TO_D2CS		0x91
#define CONNECT_CLASS_D2GS_TO_D2DBS		0x88

#define PACKET_PEER_RECV_FROM_D2CS		0x01
#define PACKET_PEER_SEND_TO_D2CS		0x02
#define PACKET_PEER_RECV_FROM_D2DBS		0x03
#define PACKET_PEER_SEND_TO_D2DBS		0x04

#endif /* INCLUDED_CONNECTION_H */