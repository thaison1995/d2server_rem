#ifndef INCLUDED_GAME_D2_PACKET_DEF_H
#define INCLUDED_GAME_D2_PACKET_DEF_H

#pragma pack(push, 1)

struct D2GSPacketSrvGeneric
{
	BYTE nHeader;			//0x00
};

struct D2GSPacketSrv26		//variable size
{
	BYTE nHeader;			//0x00
	BYTE nMessageType;		//0x01
	BYTE nLang;				//0x02
	BYTE nUnitType;			//0x03
	DWORD dwUnitGUID;		//0x04
	BYTE nMessageColor;		//0x08
	BYTE nNameColor;		//0x09
};

#pragma pack(pop)

#endif