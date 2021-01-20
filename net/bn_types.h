#ifndef INCLUDED_NET_BN_TYPES_H
#define INCLUDED_NET_BN_TYPES_H


/* basic bn types */
typedef unsigned char		bn_basic;
typedef unsigned char		bn_char;
typedef unsigned char		bn_byte;
typedef unsigned short		bn_short;
typedef unsigned int		bn_int;
typedef unsigned long		bn_long;	/* 4 bytes, not 8 bytes, fix me */


/* use network order or not? */
#ifdef USE_NBO
#define bn_htons(a)		(htons(a))
#define bn_ntohs(a)		(ntohs(a))
#define bn_htonl(a)		(htonl(a))
#define bn_ntohl(a)		(ntohl(a))
#else
#define bn_htons(a)		(a)
#define bn_ntohs(a)		(a)
#define bn_htonl(a)		(a)
#define bn_ntohl(a)		(a)
#endif

#pragma pack(push, 1)

typedef struct
{
	bn_short	size;
	bn_short	type;
	bn_int		seqno;			/* seqno, set by the sender */
} t_d2cs_d2gs_header;

typedef struct {
	bn_short	size;
	bn_short  	type;
	bn_int		seqno;			/* seqno, set by the sender */
} t_d2dbs_d2gs_header;

#pragma pack(pop)

#define CONNECT_CLASS_D2GS_TO_D2CS		0x64
#define CONNECT_CLASS_D2GS_TO_D2DBS		0x65

#define MAX_GAMENAME_LEN			16
#define MAX_GAMEPASS_LEN			16
#define MAX_GAMEDESC_LEN			32
#define MAX_ACCTNAME_LEN			16
#define MAX_CHARNAME_LEN			16
#define MAX_IPADDR_LEN			16
#define MAX_REALMNAME_LEN			32

#endif /* INCLUDED_BN_TYPES_H */