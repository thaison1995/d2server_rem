#pragma once

#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H

#include "D2DataTables.h"
#include "D2PacketDef.h"
#include <string>

struct UnitAny;
struct ClientInfo;
struct PlayerData;
struct ItemData;
struct MonsterData;
struct ObjectData;
struct Act;
struct AutoMapLayer;
struct ActMisc;
struct Path;
struct StatList;
struct Inventory;
struct Light;
struct Info;
struct Skill;
struct Damage;
struct Game;
struct ClientData;
struct Waypoint;
struct QuestInfo;
struct Room1;
struct Room2;
struct RoomTile;
struct Level;
struct PresetUnit;
struct ObjectTxt;
struct Control;
struct EditBox;
struct Image;
struct AnimImage;
struct TextBox;
struct ScrollBar;
struct Button;
struct List;
struct Arena;
struct pMsg;
struct QuestFlags;
struct CollMap;
struct CellContext;

struct StormMsg;
struct CellFile;
struct Control;
struct ScrollBar;
struct TileContext;
struct GfxCell;


struct D2Seed
{
	DWORD nLowSeed;
	DWORD nHiSeed;
};

#pragma pack(push, 1)

// Used for property build  (sizeof 0xA8)
struct DamageStats
{
	DWORD nMinDmg;
	DWORD nMaxDmg;
	DWORD dword8;
	DWORD dwordC;
	BOOL nDmgRange;
	DWORD dword14;
	DWORD nMinDmgPercent;
	DWORD nMaxDmgPercent;
	DWORD dword20;
	DWORD dword24;
	BOOL nDmgPercentRange;
	DWORD dword2C;
	DWORD nMinColdDmg;
	DWORD nMaxColdDmg;
	DWORD dword38;
	DWORD dword3C;
	DWORD nColdDmgRange;
	DWORD dword44;
	DWORD nMinLightDmg;
	DWORD nMaxLightDmg;
	DWORD dword50;
	DWORD dword54;
	DWORD nLightDmgRange;
	DWORD dword5C;
	DWORD nMinFireDmg;
	DWORD nMaxFireDmg;
	DWORD dword68;
	DWORD dword6C;
	DWORD nFireDmgRange;
	DWORD dword74;
	DWORD nMinPsnDmg;
	DWORD nMaxPsnDmg;
	DWORD nPsnLen;
	DWORD nPsnCount;
	DWORD nPsnDmgRange;
	DWORD dword8C;
	DWORD nMinMagicDmg;
	DWORD nMaxMagicDmg;
	DWORD dword98;
	DWORD dword9C;
	DWORD nMagicDmgRange;
	DWORD dwordA4;
};

// Author: Necrolis - big thx for these 2 structs (http://d2mods.info/forum/viewtopic.php?f=6&t=42044)
struct D2CharStrc      //sizeof 0x0E
{
	WORD wChar;      //+00
	BYTE nUnk;      //+02   
	BYTE nWidth;      //+03
	BYTE nHeight;      //+04
	bool bTrue;      //+05
	WORD wUnkEx;      //+06
	BYTE nImageIndex;   //+08
	BYTE nChar;      //+09
	DWORD dwUnk;      //+0A      
};

struct D2FontTable      //sizeof 0x108 ? sizeof 0xE0B
{
	DWORD dwHeader;      //+00 - 'Woo!'
	WORD wOne;      //+04
	int nLocale;      //+06
	BYTE nHeight;      //+0A
	BYTE nWidth;      //+0B
	D2CharStrc pChars[256]; //+0C
};

struct D2Font //sizeof 0x14
{
	CellFile* pFontCell;		//x000
	D2FontTable* pTable;		//0x04
	D2FontTable* pTableHeader;  //0x08
	D2CharStrc* pTableEntries;      //0x0C
	DWORD dwLastUsedTick;		//0x10
};

struct InventorySize // sizeof 0x10
{
	DWORD dwLeft;		//0x00
	DWORD dwRight;		//0x04
	DWORD dwTop;		//0x08
	DWORD dwBottom;		//0x0C
};

struct InventoryGrid // sizeof 0x18
{
	BYTE nGridX;		//0x00
	BYTE nGridY;		//0x01
	WORD _align;		//0x02
	DWORD dwLeft;		//0x04
	DWORD dwRight;		//0x08
	DWORD dwTop;		//0x0C
	DWORD dwBottom;		//0x10
	BYTE nGridWidth;	//0x14
	BYTE nGridHeight;	//0x15
	WORD _align2;		//0x16
};

struct D2WinPlacement
{
	HWND hWnd;
	WINDOWPLACEMENT hWindowPlacement;
};

struct D2Config // sizeof 0x5C
{
	char szSection[27];		// 0x00
	char szKeyName[27];		// 0x1B
	char szParameter[27];	// 0x36
	BYTE bType; 			// 0x51 - specify if config entry is byte (0) int(1) or char(2)
	BYTE _1;				// 0x52 - filler
	BYTE _2;				// 0x53 - filler
	DWORD dwStructOffset;	// 0x54 Offset on BnetData * struct
	DWORD dwDefaultValue;	// 0x58
};

struct KeyBinding //size 0xA
{
	DWORD nHotkey; // 0x00
	WORD wKey;	 // 0x04
	BOOL bPrimary;// 0x06
};

struct KeyConfigEntry //size 0xA
{
	DWORD dwEntryNo; //0x00
	WORD wLocaleNo; //0x04
	wchar_t* wText; //0x06
};

struct StormMsg
{
	HWND hWnd;
	UINT Msg;
	WPARAM wParam;
	LPARAM lParam;
	DWORD hiWParam16; // wParam shifted by 16
	BOOL _1;
	BOOL _2;
	BOOL _3;
};

struct sMsg	//size 0x0C
{
	DWORD MsgType;	//0x00   ,0 = WM_MSG , 1 = COMMAND(?), 2 = KEY_UP, 3 = KEY_DOWN
	union {
		WPARAM wParam;	//0x04
		DWORD	wKey;		//0x04
	};
	void(__stdcall* fnCallBack)(StormMsg*);	//0x08
};

#pragma pack(pop)


/*
	D2PacketTable - structure contains packet callbacks S->C
	Here is location of table for patches:
	1.11b: D2CLIENT.0xDBC30
	1.13d: D2CLIENT.0xEF4B0
*/
struct D2PacketTable
{
	BOOL(__fastcall* CallBack)(BYTE* aPacket);
	int PacketLen;
	BOOL(__fastcall* CallBack2)(UnitAny* ptUnit, BYTE* aPacket);
};


// Packet definitions

#pragma pack(push, 1)

/*
	On State Set packet
*/
struct p0xa8
{
	BYTE Header;
	BYTE UnitType;
	DWORD UnitId;
	BYTE PacketLen;
	BYTE StateNo;
	BYTE Data[1];  // Random size
};

/*
	Refresh unit packet
*/
struct px15
{
	BYTE P_15;
	BYTE UnitType;
	DWORD UnitId;
	WORD wX;
	WORD wY;
	BYTE bFlash;
};

/*
	Global message event
*/
struct px5a
{
	BYTE P_5A;		//0x00
	BYTE MsgType;	//0x01
	BYTE Color;		//0x02
	DWORD Param1;	//0x03
	BYTE Param2;	//0x07
	char Name1[16]; //0x08
	char Name2[16]; //0x18
};

/*
	Request players chat relation change
*/
struct px5d
{
	BYTE P_5D;		//0x00
	BYTE nButton;	//0x01
	BYTE nState;	//0x02
	DWORD dwUnitId; //0x03
};

/*
	Request party relation change
*/
struct px5e
{
	BYTE P_5E;		//0x00
	BYTE nButton;	//0x01
	DWORD dwUnitId; //0x02
};
#pragma pack(pop)


struct UiCallArray
{
	void(__cdecl* OnKeyDown)();
	void(__cdecl* OnKeyUp)();
	BOOL bOverride;
};

struct ItemConfig
{
	int Code;
	int	Quality;
	int Color;
	int Type;
};

struct PacketData
{
	DWORD ClientID;
	BYTE aPacket[510];
};

struct COORDS
{
	unsigned short x;
	unsigned short y;
};


struct D2RECT
{
	signed int left;
	signed int top;
	signed int right;
	signed int bottom;
};

struct D2RGB
{
	BYTE r;
	BYTE g;
	BYTE b;
};

#pragma pack(push, 1)
struct GameView // size 0xEABC
{
	DWORD dwFlags; // 0x00
	D2RECT ViewRadius; // 0x04
	D2RECT ToCheck; // 0x14
	DWORD xOffset; // 0x24
	DWORD yOffset; // 0x28
	DWORD* pGouraudTblX; // 0x2C
	DWORD* pGouraudTblY; // 0x30
	DWORD* pGouraudTblXVal; // 0x34
	DWORD _6; // 0x38
	BYTE _7[60000]; // 0x3C
	DWORD _8;
	BYTE _9[8];
	DWORD* pWall; // pointer to 36 byte structure pWall structure * nWalls
	DWORD* nWalls;
	DWORD _12;
	DWORD _13[2];
};
#pragma pack(pop)

struct GFXSettings // size 0x18
{
	DWORD bPerspectiveEnable; // 0
	DWORD bPerspectiveCapable; // 1
	DWORD bLowQuality; //2
	DWORD dwGammaValue; //3
	DWORD bVSync; //4
	DWORD bBlendedShadows; // 5
};


/*
	1.13d: 0x6FA90BFC
*/
struct GFXHelpers
{
	void(__fastcall* FillYBufferTable)(void* ppvBits, int nWidth, int nHeight, int aZero);
	void(__fastcall* DrawVisTile)(int a1, int a2, int a3);
	void(__fastcall* f3)(int a1, int a2, int a3, int a4);
	void(__fastcall* DrawGroundTile)(TileContext* tc, int xPos, int yPos, int* pLight);
	void(__fastcall* DrawWallTile)(int a1, int a2, int a3, int a4);
	void(__fastcall* DrawBlendedVisTile)(int a1, int a2, int a3, int a4);
	void(__fastcall* DrawRoofTile)(int a1, int a2, int a3, int a4, int a5);
};

#pragma pack(push, 1)
struct DT1SubBlock
{
	WORD xPos;						//0x00
	WORD yPos;						//0x02
	WORD _1;						//0x04
	BYTE gridX;						//0x06
	BYTE gridY;						//0x07
	WORD wTileFormat;				//0x08
	DWORD dwSubLength;				//0x0A
	WORD _2;						//0x0E
	DWORD* pDataOffset;				//0x10
};
#pragma pack(pop)
/*
	Big thanks to Paul Siramy for his DT1 reference
*/
struct TileContext // *.dt1
{
	DWORD dwDirection;				//0x00
	WORD wRoofIndex;				//0x04
	BYTE bSound;					//0x06
	BYTE bAnimated;					//0x07
	DWORD dwSizeY;					//0x08
	DWORD dwSizeX;					//0x0C
	DWORD dwZeros1;					//0x10
	DWORD dwOrientation;			//0x14
	DWORD dwMainIndex;				//0x18
	DWORD dwSubIndex;				//0x1C
	DWORD dwFrame;					//0x20
	BYTE  _1a;						//0x24 DT1's unknown_a ... 
	BYTE  _1c;						//0x25
	BYTE  _1b;						//0x26
	BYTE  _1d;						//0x27
	BYTE  bFlags[25];				//0x28 For each tile <not sure>
	BYTE  _2;						//0x39
	WORD sCacheIndex;				//0x40
	DWORD _18;						//0x44
	DWORD dwDataPtr;				//0x48 pointer to sub-block headers
	DWORD dwSize;					//0x4C length of the sub-blocks
	DWORD dwSubBlocks;				//0x50 
	DT1SubBlock* pBlocks;			//0x54
	char* szTileName;				//0x58
	DWORD** ptBlock;				//0x5C <not sure - maybe its a struct>
};

struct CellFile // *.dc6
{
	DWORD dwVersion;				//0x00
	struct {
		WORD dwFlags; // 0x4 = CELFILE_24BIT
		BYTE mylastcol;
		BYTE mytabno : 1;
	};								//0x04
	DWORD eFormat;					//0x08
	DWORD termination;				//0x0C
	DWORD numdirs;					//0x10
	DWORD numcells;					//0x14
	GfxCell* cells[255];			//0x18
};

struct fnRendererCallbacks
{
	BOOL(__fastcall* InitWindow)(HINSTANCE); // 0
	BOOL(__fastcall* InitPerspective)(GFXSettings* pgfxSettings, GFXHelpers* pfnHelpers); // 1
	BOOL(__fastcall* Release)(); // 2
	BOOL(__fastcall* Init)(HANDLE hWnd, int nResolution); // 3
	BOOL(__fastcall* Shutdown)(); // 4
	BOOL(__fastcall* EndCutscene)(HANDLE hWnd, int nResolution, int); // 5
	BOOL(__fastcall* BeginScene)(BOOL bClear, BYTE nRed, BYTE nGreen, BYTE nBlue); // 6
	BOOL(__fastcall* EndScene1)(); // 7
	BOOL(__fastcall* EndScene2)(); // 8
	BOOL(__fastcall* ResizeWin)(HANDLE HWND, int bResolution); // 9
	BOOL(__fastcall* GetBackBuffer)(D2RGB* pBuffer); // 10
	BOOL(__fastcall* ActivateWindow)(int Unk, int Contrast); // 11
	BOOL(__fastcall* SetOption)(int nOption, int nValue); // 12
	BOOL(__fastcall* BeginCutscene)(); // 13
	BOOL(__fastcall* PlayCutscene)(const char* szFile, int nResolution, void* pfframe); // 14
	BOOL(__fastcall* CheckCutscene)(); // 15
	BOOL(__fastcall* DecodeSmacker)(const char* szsmacker, BYTE* pbuffer, int nVersion); // 16
	BOOL(__fastcall* PlaySmacker)(void* pcontext); // 17
	BOOL(__fastcall* CloseSmacker)(void* pcontext); // 18
	int(__fastcall* GetRenderStats)(); // 19
	void(__fastcall* GetScreenSize)(int* pwidth, int* pheight); // 20
	void(__fastcall* SetScaleFactor)(int nFactor); // 21
	BOOL(__fastcall* SetGamma)(int nGamma); // 22
	BOOL(__fastcall* CheckGamma)(); // 23
	BOOL(__fastcall* SetPerspectiveScale)(); // 24
	BOOL(__fastcall* AdjustPerspective)(int nXpos, int nYpos, int nBais, int* pxadjust, int* pyadjust); // 25
	BOOL(__fastcall* ScalePerspectivepos)(int nXpos, int nYpos, int nAngle, int* pxpos, int* pypos, BOOL bOrder); // 26
	BOOL(__fastcall* SetDefperspectivefactor)(); // 27
	void(__fastcall* SetPalette)(BYTE* pPalette); // 28
	BOOL(__fastcall* SetPalettetable)(BYTE** pPalette); // 29
	BOOL(__fastcall* SetGlobalLight)(BYTE nRed, BYTE nGreen, BYTE nBlue); // 30
	BOOL(__fastcall* DrawGroundTile)(TileContext* pTile, DWORD** pLight, int nXpos, int nYpos, int nWorldXpos, int nWorldYpos, BYTE nAlpha, int nScreenPanels, BOOL bOne); // 31
	BOOL(__fastcall* DrawPerspectiveImage)(CellContext* pData, unsigned int nXpos, unsigned int nYpos, BYTE dwGamma, D2DrawModes nDrawMode, int nScreenMode, BYTE* pPalette); // 32
	BOOL(__fastcall* DrawImage)(CellContext* pData, unsigned int nXpos, unsigned int nYpos, BYTE dwGamma, D2DrawModes nDrawMode, BYTE* pPalette); // 33
	BOOL(__fastcall* DrawColouredImage)(CellContext* pData, int nXpos, int nYpos, BYTE dwGamma, D2DrawModes nDrawMode, int nColor); // 34
	BOOL(__fastcall* DrawVerticalCropImage)(CellContext* pData, int nXpos, int nYpos, int nSkipLines, int nDrawLines, D2DrawModes nDrawMode); // 35
	BOOL(__fastcall* DrawShadows)(CellContext* pData, int nXpos, int nYpos); // 36
	BOOL(__fastcall* DrawImageFast)(CellContext* pData, int nXpos, int nYpos, BYTE nPaletteIndex); // 37
	BOOL(__fastcall* DrawClippedImage)(CellContext* pData, int nXpos, int nYpos, void* pCropRect, D2DrawModes nDrawMode); // 38
	BOOL(__fastcall* DrawWallTile)(TileContext* pTile, int nXpos, int nYpos, DWORD** pLight, int nScreenPanels); // 39
	BOOL(__fastcall* DrawRoofTile)(TileContext* pTile, int nXpos, int nYpos, DWORD** pLight, int nScreenPanels, BYTE nAlpha); // 40
	BOOL(__fastcall* DrawVisTile)(TileContext* pTile, int nXpos, int nYpos, D2DrawModes nDrawMode, int nScreenPanels); // 41
	BOOL(__fastcall* DrawRect)(RECT* Prect, BYTE nColor); // 42
	BOOL(__fastcall* DrawRectEx)(RECT* Prect, BYTE nColor); // 43
	BOOL(__fastcall* DrawSolidRect)(RECT* Prect, BYTE nColor); // 44
	BOOL(__fastcall* DrawSquare)(POINT* pos, int nSize, BYTE nColor); // 45
	BOOL(__fastcall* DrawSolidRectEx)(int nXStart, int nYStart, int nXEnd, int nYEnd, BYTE nColor, D2DrawModes nDrawMode); // 46
	BOOL(__fastcall* DrawSolidRectAlpha)(int nXStart, int nYStart, int nXEnd, int nYEnd, BYTE nColor, BYTE nAlpha); // 47
	BOOL(__fastcall* DrawLine)(int nXStart, int nYStart, int nXEnd, int nYEnd, BYTE nColor, BYTE nAlpha); // 48
	BOOL(__fastcall* ClearScreen)(BOOL bPartial); // 49
	BOOL(__fastcall* DrawString)(int nXpos, int nYpos, const char* msg, ...); // 50
	BOOL(__fastcall* DrawLight)(DWORD* plight, DWORD* pplayerlight, int nXpos, int nYpos); // 51
	BOOL(__fastcall* DebugFillBackBuffer)(int xPos, int yPos); // 52
	BOOL(__fastcall* ClearCaches)(); // 53
};

/*
	Needs additional research. Start point should be on D2CLIENT.6FAF9A90 (1.13d)
	Soon to be renamed to MissileCmdStrc
*/
struct SpellStrc	//size 0x1C  // Valid for 22 -> on target & 21 -> on xy
{
	DWORD SkillId;	//0x00
	DWORD UnitId;		//0x04
	union
	{
		struct
		{
			DWORD xPos;		//0x08
			DWORD yPos;		//0x0C
		};
		struct
		{
			DWORD TargetType;	//0x08
			DWORD TargetId;		//0x0C 
		};

	};
	DWORD _1;			//0x10
	DWORD _2;			//0x14
	DWORD _3;			//0x18
};

struct PlayerCmdStrc	//size 0xC // Valid for 9 (send on PLAYER_MODE_DEAD), and probably 1-20, 23-25
{
	DWORD xPos;					//0x00
	DWORD yPos;					//0x04
	DWORD dwLastHitClass;		//0x08 usually 0
};

struct BitBuffer // Taken from Nefarius @PhrozenKeep, thx
{
	BYTE* pBuffer;			//0x00
	DWORD bitsInBuffer;		//0x04
	DWORD bufferPos;			//0x08
	DWORD bitsAtPos;			//0x0C num bits already read or written at cur pos
	DWORD unk;				//0x10 could be a bit bucket
};


struct CollMap {
	DWORD dwPosGameX;				//0x00
	DWORD dwPosGameY;				//0x04
	DWORD dwSizeGameX;				//0x08
	DWORD dwSizeGameY;				//0x0C
	DWORD dwPosRoomX;				//0x10
	DWORD dwPosRoomY;				//0x14
	DWORD dwSizeRoomX;				//0x18
	DWORD dwSizeRoomY;				//0x1C
	WORD* pMapStart;				//0x20
	WORD* pMapEnd;					//0x22
};


struct cStylePrefs
{
	DWORD dwFontType;	//0x00
	DWORD dwColor;		//0x04
};

struct cTextPrefs
{
	DWORD dwPosX;		//0x00
	DWORD dwPosY;		//0x04
	DWORD dwSizeY;		//0x08
};


struct ControlText // size = 0x20
{
	wchar_t* wText[5]; 	//0x00 for each field
	DWORD dwColor;		//0x14
	DWORD dwAlign;		//0x18
	ControlText* pNext;	//0x1C
};

struct ControlMsg {
	Control* pControl;
	UINT uMsg;
	WPARAM wParam;
};

struct Control {
	DWORD dwType;                                                          //0x00
	CellFile* pCellFile;                                                   //0x04
	DWORD dwState;                                                         //0x08 0x05 - enabled, 0x04 - disabled, 0x03-0x00 - not visible
	DWORD dwPosX;                                                          //0x0C
	DWORD dwPosY;                                                          //0x10
	DWORD dwSizeX;                                                         //0x14
	DWORD dwSizeY;                                                         //0x18
	BOOL(__fastcall* Draw)(Control*);								    //0x1C
	BOOL(__fastcall* DrawEx)(Control*);						  	        //0x20 used by timer/popup
	BOOL(__stdcall* Push)(ControlMsg*);							        //0x24 
	BOOL(__stdcall* Mouse)(ControlMsg*);				            		//0x28 
	BOOL(__stdcall* ListCheck)(ControlMsg*);                          	//0x2C used only by list
	BOOL(__stdcall* Key)(ControlMsg*);							 	    //0x30 WM_CHAR MSG
	BOOL(__stdcall* OnPress)(Control*);		                            //0x34 User Defined
	BOOL(__fastcall* DrawAnim)(Control*);                          		//0x38 used by animimage
	Control* pNext;                                                        //0x3C
};

struct EditBox : Control //(size 0x284)
{
	//	 Control Header;														//0x00
	DWORD dwLeftOffset;												    //0x40 
	DWORD dwTopOffset;													    //0x44
	DWORD dwMaxLength;                                   	                //0x48 
	DWORD dwTextOffset;	                              		            //0x4C 
	DWORD dwTextLen;														//0x50 (strlen -1)
	DWORD dwSelectEnd;                                                     //0x54
	DWORD dwSelectStart;                                                   //0x58
	wchar_t wText[256];													//0x5C
	DWORD dwCursorPos;										  			    //0x25C
	DWORD dwEditFlags;														//0x260 0x08 allows multiline
	BOOL(__stdcall* OnAccept)(char*);										//0x264
	BOOL(__stdcall* InputHandle)(Control*, DWORD len, char* Key);			//0x268
	BOOL(__stdcall* LengthHandle)(int aNull);								//0x26C hmm weird arg =  always 0
	cStylePrefs Style;														//0x270 
	Control* pTabNext;													//0x278
	Control* pTabPrev;													//0x27C
	BOOL  bLeftButtonPressed;												//0x280
};

struct TextBox : Control // (size 0xAC)
{
	//	 Control Header;									//0x00
	DWORD dwLeftOffset;								//0x40 
	DWORD dwTopOffset;								    //0x44
	ControlText* pFirstText;                           //0x48
	ControlText* pLastText;                            //0x4C
	ControlText* pSelectedText;                        //0x50
	DWORD dwMaxLines;									//0x54
	DWORD dwCurrentLine;								//0x58
	DWORD dwTextFlags;									//0x5C 0x00 - left align 0x01 - SelectBox 0x2 - center 0x4 Create ScrollBar 0x40 - scrolling from right 0x80 - scrolling from bottom
	DWORD dwSelectedItem;								//0x60
	DWORD dwFields;									//0x64
	DWORD dwFieldXSize[5];								//0x68 for each field
	DWORD dwFieldAlign[5];								//0x7C
	ScrollBar* ptScrollBar;							//0x90
	cStylePrefs Style;									//0x94
	DWORD dwMaxTextWidth;								//0x9C
	DWORD dwMaxTextHeight;								//0xA0
	DWORD dwInterline;									//0xA4
	DWORD dwInterline2;								//0xA8
};

struct Image : Control // (size 0x4C)
{
	//	Control Header;										//0x00
	DWORD CellFrame;									//0x40
	DWORD TransLvl;										//0x44
	void* _1;											//0x48 image struct?
};

struct AnimImage : Control // 0x60
{
	//	Control Header;										//0x00
	void* AnimStruct;									//0x40 pointer to struct which holds all cellfile frames
	DWORD dwAnimSpeed;									//0x44
	DWORD dwTickCount;									//0x48 creation time
	DWORD dwCurrentFrame;								//0x4C
	BOOL bisHovered;									//0x50
	DWORD dwTransLvl;									//0x54
	void(__stdcall* OnHover)(AnimImage*);				//0x58
	BOOL bisAnimation;									//0x5C
};

struct Button : Control // (size 0x274)
{
	//	 Control Header;									//0x00
	DWORD dwButtonFlags;								//0x40 0x00 - normal 0x01 - radio 0x02 - switch 0x04 - play sound 0x20 - sth with disabled/enabled 0x40 - multi line 
	DWORD dwIsPushed;									//0x44
	BOOL  dwIsSwitched;                                //0x48
	BOOL _1;			                                //0x4C
	DWORD dwHotKey;                             	    //0x50
	DWORD dwButtonType;                           		//0x54 0x00 - Normal Button, 0x01 - Switch Button 0x02 - Long Button
	DWORD _2;	                                        //0x58
	DWORD dwCellFrame;                                 //0x5C
	DWORD dwFont;										//0x60  hardcoded
	wchar_t wText[256];                              	//0x64
	DWORD dwColor;										//0x264 hardcoded
	BOOL(__stdcall* OnHover)(Button*);				//0x268
	BOOL bisHovered;									//0x26C
	DWORD dwStrTbl2ndLine;								//0x270
};

struct ScrollBar : Control // (size 0x60)
{
	//	 Control Header;									//0x00
	BOOL bMovedUp;	                                    //0x40 
	BOOL bMovedDown;	                                //0x44
	DWORD dwScrollEntries;								//0x48
	DWORD dwScrollPosition;                            //0x4C
	DWORD dwClickStep;                                 //0x50
	TextBox* ptParent;								//0x54
	BOOL  bLeftButtonPressed;							//0x58
	BOOL(__stdcall* Unk)(ControlMsg*);				//0x5C
};

struct List : Control // (size 0x6C)
{
	//	Control Header;										//0x00
	DWORD dwFont;										//0x40
	DWORD _2;											//0x44
	DWORD _3;											//0x48
	DWORD _4;											//0x4C
	DWORD _5;											//0x50
	DWORD _6;											//0x54
	DWORD _7;											//0x58
	DWORD _8;											//0x5C
	DWORD _9;											//0x60
	DWORD* _10;											//0x64
	DWORD _11;											//0x68	
};

struct D2Menu // size 0x18
{
	DWORD dwEntriesNo;		//0x00
	DWORD dwInterline;		//0x04
	DWORD dwTextHeight;		//0x08
	DWORD dwMenuOffset;		//0x0C
	DWORD dwBarHeight;		//0x10
	DWORD _1;				//0x14 unused?
};

struct D2MenuEntry //size 0x550
{
	DWORD dwMenuType;									 //0x00  //-1 - static text, 0 -selectable, 1- switchbar , 2- with bar, 3 - key config (added), 4 - switch bar ex (added)
	DWORD dwExpansion;									 //0x04  //if set, shows only in d2exp
	DWORD dwYOffset;									 //0x08  //generated dynamicaly
	union {
		char szCellFile[260];								 //0x0C  DATA\\LOCAL\\UI\\LANG\\%s used only in LoadMenu Func
		wchar_t wItemName[130];								 //my addition
	};
	BOOL(__fastcall* EnableCheck)(D2MenuEntry*, DWORD ItemNo); //0x110 if return false, its disabled
	BOOL(__fastcall* OnPress)(D2MenuEntry*, StormMsg*);		 //0x114 28.12.11 - added StormMsg*
	BOOL(__fastcall* OnChange)(D2MenuEntry*);				 //0x118 called when option value is changed (renamed from OptionHandle) also sets default value for switch on init
	BOOL(__fastcall* ValidateCheck)(D2MenuEntry*);			 //0x11C if return true OnPress is called, and option gfx is switched (renamed from ChangeHandle)
	union {
		DWORD dwMaxValue;									 //0x120
		DWORD dwSwitchesNo;									 //0x120  (max 4 for switchbar, 260 for switchbarex)
		DWORD* Bind;										 //0x120 -> KeyConfig (my add)
	};
	union {
		DWORD dwCurrentValue;								 //0x124
		DWORD dwCurrentSwitch;
		DWORD dwColor;										 //my addition ONLY for MenuType = -1, 1, 3(?)
	};
	union {
		DWORD dwBarType;									 //0x128
		DWORD dwFontType;									 //my add valid for all
	};
	union {
		char szSwitchCellFiles[4][260];						//0x12C DATA\\LOCAL\\UI\\LANG\\%s used only in LoadMenu Func
		wchar_t wSwitchItemName[4][130];					//my addition
		std::wstring* wSwitches[260];							//my addition for menu type = 4
	};
	CellFile* ptCellFile;								 //0x53C
	CellFile* ptSwitchCellFile[4];						 //0x540
};



struct HoverText	//size 0x110
{
	DWORD dwDisplayTime;	//0x00	8 * strlen + 125
	DWORD dwExpireTime;		//0x04	DisplayTime+CurrentTick;
	DWORD dwLangId;			//0x08
	DWORD _1;				//0x0C	Unused?
	char szMsg[255];		//0x10
};

struct ChatMsg		//size 0x2C
{
	wchar_t* wMsg;			//0x00
	void* _1;				//0x04
	//...
};

struct Inventory
{
	DWORD dwSignature;				//0x00
	BYTE* pMemPool;					//0x04
	UnitAny* pOwner;				//0x08
	UnitAny* pFirstItem;			//0x0C
	UnitAny* pLastItem;				//0x10
	DWORD _1[2];					//0x14
	DWORD dwLeftItemUid;			//0x1C
	UnitAny* pCursorItem;			//0x20
	DWORD dwOwnerId;				//0x24
	DWORD dwItemCount;				//0x28
	DWORD _2[4];					//0x2C
	DWORD CorpseNumber;				//0x3C
};

struct ItemData {
	DWORD QualityNo;		//0x00
	DWORD LowSeed;			//0x04
	DWORD HighSeed;			//0x08
	DWORD OwnerID;			//0x0C
	DWORD InitSeed;			//0x10
	DWORD CommandFlags;		//0x14
	DWORD ItemFlags;		//0x18
	DWORD _1[2];			//0x1C
	DWORD ActionStamp;		//0x24
	DWORD FileIndex;		//0x28
	DWORD ItemLevel;		//0x2C
	WORD ItemFormat;		//0x30
	WORD RarePrefix;		//0x32
	WORD RareSutfix;		//0x34
	WORD AutoPrefix;		//0x36
	WORD MagicPrefix[3];	//0x38
	WORD MagicSutfix[3];	//0x3E
	BYTE BodyLoc;		    //0x44					// location on body = xpos of item (only if on body) (spread)
	BYTE InvPage;			//0x45					// 0 = inventory,  3= cube, 4=stash (spread)
	BYTE _2[2];				//0x46
	BYTE EarLvl;			//0x48
	BYTE InvGfxIdx;			//0x49
	char szPlayerName[16];	//0x4A
	BYTE _3[2];				//0x5A
	Inventory* pNodeInv;	//0x5C
	DWORD _4;				//0x60
	UnitAny* pNextItem;		//0x64
	BYTE NodePos;			//0x68
	BYTE NodePosOther;		//0x69
};

struct ActEnvironment // sizeof(0x38)
{
	DWORD _1;						//0x00  2
	DWORD _2;						//0x04  0
	DWORD _3;						//0x08  0 * 128
	DWORD _4;						//0x0C  32
	DWORD dwCreatedTick;			//0x10
	DWORD _6;						//0x14
	DWORD _7;						//0x18  Some angle (sin( (_3 / _11)  * (1/180) * PI)
	DWORD _8;						//0x1C  Some angle (-cos( (_3 / _11) * (1/180) * PI)
	DWORD _9;						//0x20
	DWORD _10;						//0x24
	DWORD _11;						//0x28  128
	DWORD _12;						//0x2C
	DWORD _13;						//0x30
	DWORD _14;						//0x34
};

//sizeof = 0x60
struct Act
{
	DWORD _1a;						//0x00
	ActEnvironment* pEnviroment;	//0x04
	DWORD dwTownLvl;				//0x08
	DWORD dwMapSeed;				//0x0C
	Room1* pRoom1;					//0x10
	DWORD dwAct;					//0x14
	DWORD hTile[12];				//0x18 not sure, some inline struct of 0x30 size seems fit
	ActMisc* pMisc;					//0x48
	DWORD _4;						//0x4C
	DWORD _5;						//0x50
	DWORD _6;						//0x54
	DWORD _7;						//0x58
	void* pMemPool;					//0x5C
};


// sizeof 0x48
struct DrlgVis
{
	DWORD nLevelDef;		//0x00 -> record number of LevelDefs.bin
	DWORD dwVis[8];			//0x04
	DWORD dwWarp[8];		//0x24
	DrlgVis* pNext;			//0x44
};

struct ActMisc // sizeof(0x48C) aka Drlg
{
	D2Seed hSeed;			//0x00
	DWORD _1;				//0x08
	DWORD* pTile;			//0x0C
	DWORD _1g[15];			//0x10
	DWORD pfnCallBack;		//0x4c
	DWORD _1a[16];			//0x50
	DrlgVis* pVis;			//0x90
	DWORD dwStaffTombLevel; //0x94
	DWORD _2a;				//0x98
	Game* pGame;			//0x9C
	DWORD _2[236];			//0xA0
	DWORD nDiffLvl;			//0x450
	DWORD* fnCallback;		//0x454
	DWORD InitSeed;			//0x458
	DWORD _2d[4];			//0x45C
	Act* pAct;				//0x46C
	DWORD _3[2];			//0x470
	void* pMemPool;			//0x478 D2PoolManager*
	Level* pLevelFirst;		//0x47C
	DWORD nAct;				//0x480
	DWORD _5;				//0x484 Tomb Levels Related 66 + rand(7)
	DWORD* fnCallback2;		//0x488
};

struct AutoMapCell
{
	DWORD fSaved;					//0x00
	WORD nCellNo;					//0x04
	WORD xPixel;					//0x06
	WORD yPixel;					//0x08
	WORD wWeight;					//0x0A
	AutoMapCell* pLess;				//0x0C
	AutoMapCell* pMore;				//0x10
};

struct AutoMapLayer //sizeof = 0x1C
{
	DWORD nLayerNo;					//0x00
	DWORD fSaved;					//0x04
	AutoMapCell* pFloors;			//0x08
	AutoMapCell* pWalls;			//0x0C
	AutoMapCell* pObjects;			//0x10
	AutoMapCell* pExtras;			//0x14
	AutoMapLayer* pNextLayer;		//0x18
};

struct AutoMapLayer2
{
	DWORD _1[2];					//0x00
	DWORD nLayerNo;					//0x08
};

struct PresetUnit {
	DWORD _1;						//0x00
	DWORD dwClassId;				//0x04
	DWORD dwPosX;					//0x08
	PresetUnit* pPresetNext;		//0x0C
	DWORD _3;						//0x10
	DWORD dwType;					//0x14
	DWORD dwPosY;					//0x18
};


struct RoomCoords // sizeof(0x20) 32 bytes of Room1+4C
{
	DWORD dwXStart;					//0x00
	DWORD dwYStart;					//0x04
	DWORD dwXSize;					//0x08
	DWORD dwYSize;					//0x0C
	WORD wTileX;					//0x10
	WORD _1b;						//0x12
	WORD wTileY;					//0x14
	WORD _2b;						//0x16
	DWORD _3;						//0x18
	DWORD _4;						//0x1C
};


struct Room1 {
	Room1** pRoomsNear; 	//0x00 pptVisibleRooms
	DWORD _1;				//0x04
	void* _1s;				//0x08
	DWORD _1b;				//0x0C
	Room2* pRoom2;			//0x10
	DWORD _2[2];			//0x14
	UnitAny** pUnitChanged; //0x1C
	CollMap* Coll;			//0x20
	DWORD dwRoomsNear;		//0x24 dwVisibleRooms
	DWORD nPlayerUnits;		//0x28
	Act* pAct;				//0x2C
	DWORD _4;				//0x30
	DWORD nUnknown;			//0x34
	DWORD _5[4];			//0x38
	ClientData** pClients;  //0x48
	RoomCoords hCoords;		//0x4C
	D2Seed hSeed;			//0x6C
	UnitAny* pUnitFirst;	//0x74
	DWORD nNumClients;		//0x78
	Room1* pRoomNext;		//0x7C
};

struct Room2 {
	DWORD _1[2];			//0x00
	Room2** pRoom2Near;		//0x08
	DWORD _2[2];			//0x0C
	D2Seed hSeed;			//0x14
	DWORD _2b;				//0x1C
	struct {
		DWORD dwRoomNumber; //0x00
		DWORD* _1;			//0x04
		DWORD* pdwSubNumber;//0x08
	} *pType2Info;			//0x20 <- points to 0x70 struct if dwPresetType == 1, 0xF8 struct if == 2
	Room2* pRoom2Next;		//0x24
	DWORD dwRoomFlags;		//0x28
	DWORD dwRoomsNear;		//0x2C
	Room1* pRoom1;			//0x30
	DWORD dwPosX;			//0x34
	DWORD dwPosY;			//0x38
	DWORD dwSizeX;			//0x3C
	DWORD dwSizeY;			//0x40
	DWORD _3;				//0x44
	DWORD dwPresetType;		//0x48
	RoomTile* pRoomTiles;	//0x4C
	DWORD _4[2];			//0x50
	Level* pLevel;			//0x58
	PresetUnit* pPreset;	//0x5C
};

struct RoomTile {
	Room2* pRoom2;				//0x00
	RoomTile* pNext; 			//0x04
	DWORD _2[2];				//0x08
	DWORD* nNum; 				//0x10
};


//sizeof = 0x8
struct PresetData
{
	DWORD _1;					//0x00
	DWORD dwPreset;				//0x04 -> randomly selected from Files columun in LvlPrest.txt
};

//sizeof = 0x10
struct LevelPreset
{
	DWORD dwDef;				//0x00
	DWORD dwFiles;				//0x04 number of presets
	DWORD dwPreset;				//0x08 randomly selected from Files columun in LvlPrest.txt
	LevelPreset* pNext;			//0x0C
};


// sizeof = 0x38
struct WildernessUnk
{
	Level* pLevel;					//0x00
	DWORD _2;						//0x04
	DWORD dwRooms;					//0x08
	DWORD _4;						//0x0C
	DWORD _5;						//0x10
	DWORD _6;						//0x14
	DWORD dwType;					//0x18 pLvlSub.txt type
	DWORD fnCallback[7];			//0x1C
};


struct DrlgGrid // size 0x14
{
	DWORD* gridX;			//0x00
	DWORD* gridY;			//0x04 points to memory of (4 * ySize * (xSize + 1)) 
	DWORD dwXSize;			//0x08
	DWORD dwYSize;			//0x0C
	DWORD _5;				//0x10
};

//sizeof = 0x268
struct WildernessData
{
	DWORD dwFlags;				//0x00
	DrlgGrid hGrid[4];			//0x08
	DWORD  _12[2];				//0x0C
	DWORD dwSizeX;				// LevelSizeX / 8
	DWORD dwSizeY;				// LevelSizeY / 8
	DWORD  _13[7];				//0x0C
	DWORD _UnkArr[4][30];		//0x80
	DWORD _14;					//0x260
	DWORD _15;					//0x264
};

// sizeof = 0x230
struct Level {
	DWORD dwDrlgType;			//0x00 1 - maze, 2 - preset, 3 - wilderness
	DWORD dwLevelFlags;			//0x04
	DWORD _1[2];				//0x08
	Room2* pRoom2First;			//0x10
	union {
		LvlMazeTxt* pMazeTxt;    		//     for dwDrlgType == 1 (RANDOM MAZE)
		PresetData* pPreset;			//     for dwDrlgType == 2 (PRESET MAP)
		WildernessData* pWilderness;	//     for dwDrlgType == 3 (RANDOM AREA WITH PRESET SIZE)
	}; // 0x14
	DWORD _2;					//0x18
	DWORD dwPosX;				//0x1C
	DWORD dwPosY;				//0x20
	DWORD dwSizeX;				//0x24
	DWORD dwSizeY;				//0x28
	DWORD _3[96];				//0x2C
	Level* pNextLevel;			//0x1AC
	DWORD _4;					//0x1B0
	ActMisc* pMisc;				//0x1B4
	DWORD _5[2];				//0x1B8
	DWORD dwLevelType;			//0x1C0
	D2Seed hSeed;				//0x1C4
	LevelPreset* pLevelPresets;	//0x1CC
	DWORD dwLevelNo;			//0x1D0
	DWORD _6[3];				//0x1D4
	union {
		DWORD RoomCenterX[9];
		DWORD WarpX[9];
	};							//0x1E0
	union {
		DWORD RoomCenterY[9];
		DWORD WarpY[9];
	};							//0x204
	DWORD dwRoomEntries;		//0x228
	DWORD _7;					//0x22C
};

struct Path	//sizeof 0x200
{
	short xOffset;					//0x00
	short xPos;						//0x02
	short yOffset;					//0x04
	short yPos;						//0x06
	DWORD xUnknown;					//0x08  16 * (wInitX - wInitY) <- Mby AutomapX
	DWORD yUnknown;					//0x0C  8 * (wInitX + wInitY + 1) <- Mby AutoampY
	short xTarget;					//0x10
	short yTarget;					//0x12
	DWORD _2[2];					//0x14
	Room1* pRoom1;					//0x1C
	Room1* pRoomUnk;				//0x20
	DWORD _3[3];					//0x24
	UnitAny* pUnit;					//0x30
	DWORD dwFlags;					//0x34 0x40000 -> PATH_MISSILE_MASK
	DWORD _4;						//0x38
	DWORD dwPathType;				//0x3C
	DWORD dwPrevPathType;			//0x40
	DWORD dwUnitSize;				//0x44
	DWORD _5[2];					//0x48
	DWORD dwCollisionFlag;			//0x50  0x1804 <- bFlying, 0x3401 <- bOpenDoors, 0x3C01 <- Cannot Open Doors, 0x804 <- Ghost, 0x1C09 <- Player
	DWORD _5d;						//0x54
	UnitAny* pTargetUnit;			//0x58
	DWORD dwTargetType;				//0x5C
	DWORD dwTargetId;				//0x60
	BYTE bDirection;				//0x64
};

struct StaticPath // size 0x20
{
	Room1* pRoom1;		//0x00
	DWORD xOffset;		//0x04
	DWORD yOffset;		//0x08
	DWORD xPos;			//0x0C
	DWORD yPos;			//0x10
	DWORD _1[2];			//0x14
	DWORD dwFlags;		//0x1C
};


struct PlayerData { //size 0x16C , copied from serverside, dunno if everything is the same on client
	char szName[16];				//0x00
	QuestFlags* QuestsFlags[3];		//0x10
	Waypoint* pNormalWaypoint[3];	//0x1C
	void* ptAiGeneral;				//0x28
	DWORD _1[2];					//0x2C
	WORD* KillCounter;				//0x34 Monster
	DWORD _1b;						//0x38
	DWORD _2[4];					//0x3C 2[2] is merc related
	DWORD dwTradeTick;				//0x4C mby InactivityTime
	DWORD nTradeState;				//0x50
	DWORD _3;						//0x54
	DWORD dwAcceptTradeTick;		//0x58
	void* pTrade;					//0x5C
	DWORD _4[3];					//0x60
	DWORD dwBoughtItemId;			//0x6C
	DWORD dwRightSkill;				//0x70
	DWORD dwLeftSkill;				//0x74
	DWORD dwRightSkillFlags;		//0x78
	DWORD dwLeftSkillFlags;			//0x7C
	DWORD dwSwitchRightSkill;		//0x80
	DWORD dwSwitchLeftSkill;		//0x84
	DWORD dwSwitchRightSkillFlags;	//0x88
	DWORD dwSwitchLeftSkillFlags;	//0x8C
	DWORD _5[3];					//0x90
	ClientData* pClientData;		//0x9C
	DWORD _6[50];					//0x100
	DWORD GameFrame;				//0x168
};

struct ObjectData
{
	ObjectTxt* pObjectTxt;				//0x00
	BYTE InteractType;				//0x04
	BYTE _1[3];						//0x05
	DWORD _2[8];					//0x08
	char szOwner[16];				//0x28
};

struct AiGeneral
{
	DWORD SpecialState;										  //0x00 - stuff like terror, confusion goes here
	void* (__fastcall* fpAiFunction)(Game*, UnitAny*, DWORD*);  //0x04 - the primary ai function to call
	DWORD AiFlags;											  //0x08
	DWORD OwnerGUID;											  //0x0C- the global unique identifier of the boss or minion owner
	DWORD eOwnerType;											  //0x10 - the unit type of the boss or minion owner
	DWORD dwArgs[3];											  //0x14 - three dwords holding custom data used by ai func to store counters (etc)
	DWORD* pCmdCurrent;										  //0x20
	DWORD* pCmdLast;											  //0x24
	Game* pGame;												  //0x28
	DWORD OwnerGUIDEq;										  //0x2C - the same as +008
	DWORD eOwnerTypeEq;										  //0x30 - the same as +00C
	DWORD* pMinionList;										  //0x34 - list of all minions, for boss units (SetBoss in MonStats, Unique, SuperUnique etc)
	DWORD _1;													  //0x38
	DWORD eTrapNo;											  //0x3C - used by shadows for summoning traps (so they stick to one type usually)
};

struct MonsterData {
	MonStatsTxt* pMonStatsTxt;				//0x00
	BYTE Components[16];					//0x04
	WORD NameSeed;							//0x14
	struct
	{
		BYTE fOther : 1;
		BYTE fSuperUniq : 1;
		BYTE fChampion : 1;
		BYTE fUnique : 1;
		BYTE fMinion : 1;
		BYTE fPossesed : 1;
		BYTE fGhostly : 1;
		BYTE fMultishot : 1;
	};				//0x16
	BYTE dwLastMode;				//0x17
	DWORD dwDuriel;					//0x18
	BYTE anEnchants[9];				//0x1C
	BYTE _1;						//0x25
	WORD wUniqueNo;					//0x26
	AiGeneral* pAiGeneral;			//0x28
	wchar_t* wName[28];				//0x2C
};


struct Stat // size 0x8
{
	WORD wSubIndex;					//0x00
	WORD wStatIndex;				//0x02
	DWORD dwStatValue;				//0x04
};

struct StatEx	//size 0x8
{
	Stat* pStat;					//0x00 An Array[wStatCount]
	WORD wStatCount;				//0x04
	WORD wnBitSize;					//0x06
};

struct StatListEx  //size 0x64
{
	DWORD pMemPool;					//0x00
	DWORD _1;						//0x04
	DWORD dwOwnerType;				//0x08
	DWORD dwOwnerId;				//0x0C
	DWORD dwListFlags;				//0x10
	DWORD _2[4];					//0x14
	StatEx BaseStats;				//0x24
	StatList* pLastList;			//0x2C - pointer to the last pStatList of the StatListEx owner (aka item owner in case list ex belongs to item)
	DWORD _3;						//0x30
	StatListEx* pListEx;			//0x34 - pStatListEx - pointer to owner StatListEx (if this one is owned by a item, this points to the item owners list)
	StatListEx* pNextListEx;		//0x38
	StatList* pMyLastList;			//0x3C
	StatList* pMyStats;				//0x40
	UnitAny* pOwner;				//0x44
	StatEx FullStats;				//0x48
	StatEx ModStats;				//0x50
	BYTE* StatFlags;				//0x58 pt to an  array
	void* fpCallBack;				//0x5C
	Game* ptGame;					//0x60
};

struct StatList { //size 0x40
	void* pMemPool;					//0x00
	UnitAny* pUnit;					//0x04
	DWORD dwOwnerType;				//0x08
	DWORD dwOwnerId;				//0x0C
	DWORD dwFlags;					//0x10
	DWORD dwStateNo;				//0x14
	DWORD ExpireFrame;				//0x18
	DWORD SkillNo;					//0x1C
	DWORD sLvl;						//0x20
	StatEx Stats;					//0x24
	StatList* pPrevLink;			//0x2C
	StatList* pNextLink;			//0x30
	StatList* pPrev;				//0x34
	void* fpStatRemove;				//0x38
	StatList* pNext;				//0x3C
};

struct GfxDrawOffset
{
	DWORD DrawFlags;				//0x00
	DWORD xPos;						//0x04
	DWORD yPos;						//0x08
	DWORD zPos;						//0x0C
	struct
	{
		DWORD xOffset;				//0x00
		DWORD yOffset;				//0x04
		DWORD zOffset;				//0x08
	} Offsets[3];					//0x10
};

struct GfxInfo
{
	DWORD _1;						//0x00
	DWORD _2;						//0x04
	DWORD _3;						//0x08
	DWORD _4;						//0x0C
	DWORD _5;						//0x10
	DWORD _6;						//0x14
	DWORD _7;						//0x18
	DWORD _8;						//0x1C
	DWORD _9;						//0x20
	DWORD _10;						//0x24
	DWORD _11;						//0x28
	DWORD _12;						//0x2C
	GfxDrawOffset* pGfxDrawOffsets;//0x30
	DWORD _13;						//0x34
	DWORD nPalShiftIdx;				//0x38
};

struct UnitAny  // CLIENT SIDE
{
	DWORD dwType;					//0x00
	DWORD dwClassId;				//0x04
	void* pMemPool;					//0x08
	DWORD dwUnitId;					//0x0C
	DWORD dwMode;					//0x10
	union
	{
		PlayerData* pPlayerData;
		ItemData* pItemData;
		MonsterData* pMonsterData;
		ObjectData* pObjectData;
	};								//0x14
	DWORD dwAct;					//0x18
	Act* pAct;						//0x1C
	DWORD dwSeed[2];				//0x20
	DWORD dwInitSeed;				//0x28
	union
	{
		Path* pPath;
		StaticPath* pStaticPath;
	};								//0x2C
	DWORD _3[5];					//0x30
	DWORD dwGfxFrame;				//0x44
	DWORD dwFrameRemain;			//0x48
	WORD wFrameRate;				//0x4C
	WORD _4;						//0x4E
	DWORD* pAnimData;				//0x50
	GfxInfo* pGfxInfo;				//0x54
	GfxInfo* pGfxInfo2;				//0x58
	StatListEx* pStatsEx;			//0x5C
	Inventory* pInventory;			//0x60
	Light* pLight;					//0x64
	DWORD  dwLightRadious;			//0x68
	WORD   nPalShiftIdx;			//0x6C
	WORD UpdateType;				//0x6E
	UnitAny* pUpdateUnit;			//0x70
	DWORD* pQuestRecord;			//0x74
	BYTE bSparkyChest;				//0x78
	BYTE _6a[3];					//0x79
	DWORD pTimerArgs;				//0x7C
	DWORD dwSoundSync;				//0x80
	DWORD _7[2];					//0x84
	WORD wX;						//0x8C
	WORD wY;						//0x8E
	DWORD pEvent;					//0x90
	DWORD dwOwnerType;				//0x94
	DWORD dwOwnerId;				//0x98
	DWORD _8[2];					//0x9C
	HoverText* pHoverText;			//0xA4
	Info* pInfo;					//0xA8
	void* ptCombat;					//0xAC
	DWORD dwLastHitClass;			//0xB0
	DWORD _9[4];					//0xB4
	DWORD dwFlags;					//0xC4
	DWORD dwFlags2;					//0xC8
	DWORD _10[5];					//0xCC
	UnitAny* pChangedNext;			//0xE0
	UnitAny* pRoomNext;			//0xE4 // changed 14.12.11
	UnitAny* pListNext;			//0xE8
	pMsg* pMsgFirst;				//0xEC
	pMsg* pMsgLast;					//0xF0
};

struct Skill
{
	SkillsTxt* pSkillsTxt;			//0x00
	Skill* pNextSkill;				//0x04
	DWORD SkillMode;				//0x08
	DWORD _1[7];					//0x0C
	DWORD dwSkillLevel;				//0x28
	DWORD _2[2];					//0x2C
	DWORD dwFlags;					//0x30
};

struct Info
{
	DWORD* pMemPool;			//0x00
	Skill* pFirstSkill;			//0x04
	Skill* pLeftSkill;			//0x08
	Skill* pRightSkill;			//0x0C
};

#pragma pack(push, 1)
struct BnetData // sizeof 0x3C8 name of this structure is a big mistake, but I won't change it at this stage...it's more like clientside version of ClientData
{
	DWORD bExpansion;				//0x00
	BYTE bWindowMode;				//0x04
	BYTE bFixAspectRatio;			//0x05
	BYTE b3DFXMode;					//0x06
	BYTE bOpenGLMode;				//0x07
	BYTE bRaveMode;					//0x08
	BYTE bDirect3DMode;				//0x09
	BYTE bUsePerspective;			//0x0A
	BYTE bLowQuality;				//0x0B 
	DWORD dwGamma;					//0x0C
	BYTE bVSync;					//0x10
	DWORD dwFrameRate;				//0x11
	DWORD dwGameType;				//0x15 Values 1, 2 , 3 , 6 , 7
	WORD wJoinId;					//0x19 Overflows on GameName (parameter is an int)
	char szGameName[24];			//0x1B Overflows on GameIP (every string here should've 27 length)
	char szGameIP[24];				//0x33 Overflows -||-
	char szBattleNetIp[24];			//0x4B -||-
	char szMCPIp[24];				//0x63 
	BYTE _8a[4];					//0x7B
	BYTE bNoPK;						//0x7F
	BYTE bOpenC;					//0x80 -openc
	BYTE bAmazon;					//0x81 default is true
	BYTE bPaladin;					//0x82
	BYTE bSorceress;				//0x83
	BYTE bNecormancer;				//0x84
	BYTE bBarbarian;				//0x85
	BYTE _8b;						//0x86
	BYTE _8c;						//0x87
	BYTE bInvincible;				//0x88
	char szAccountName[48];			//0x89
	char szPlayerName[24];			//0xB9
	char szRealmName[27];			//0xD1
	DWORD _11;						//0xEC  <- splited out these bytes to figure out rest of stuff
	DWORD _12;						//0xF0
	DWORD _13;						//0xF4
	DWORD _14;						//0xF8
	DWORD _15;						//0xFC
	DWORD _16;						//0x100
	DWORD _17;						//0x104
	DWORD _18;						//0x108
	DWORD _19;						//0x10C
	DWORD _20;						//0x110
	DWORD _21;						//0x114
	DWORD _22;						//0x118
	DWORD _23;						//0x11C
	DWORD _24;						//0x120
	DWORD _25;						//0x124
	DWORD _26;						//0x128
	DWORD _27;						//0x12C
	DWORD _28;						//0x130
	DWORD _29;						//0x134
	DWORD _30;						//0x138
	DWORD _31;						//0x13C
	DWORD _32;						//0x140
	DWORD _33;						//0x144
	DWORD _34;						//0x148
	DWORD _35;						//0x14C
	DWORD _36;						//0x150
	DWORD _37;						//0x154
	DWORD _38;						//0x158
	DWORD _39;						//0x15C
	DWORD _40;						//0x160
	DWORD _41;						//0x164
	DWORD _42;						//0x168
	DWORD _43;						//0x16C
	DWORD _44;						//0x170
	DWORD _45;						//0x174
	DWORD _46;						//0x178
	DWORD _47;						//0x17C
	DWORD _48;						//0x180
	DWORD _49;						//0x184
	DWORD _50;						//0x188
	DWORD _51;						//0x18C
	DWORD _52;						//0x190
	DWORD _53;						//0x194
	DWORD _54;						//0x198
	DWORD _55;						//0x19C
	DWORD _56;						//0x1A0
	DWORD _57;						//0x1A4
	DWORD _58;						//0x1A8
	DWORD _59;						//0x1AC
	DWORD _60;						//0x1B0
	DWORD _61;						//0x1B4
	DWORD _62;						//0x1B8
	DWORD _63;						//0x1BC
	DWORD _64;						//0x1C0
	DWORD _65;						//0x1C4
	DWORD _66;						//0x1C8
	DWORD _67;						//0x1CC
	DWORD _68;						//0x1D0
	DWORD _69;						//0x1D4
	DWORD _70;						//0x1D8
	DWORD _71;						//0x1DC
	DWORD _72;						//0x1E0
	DWORD _73;						//0x1E4
	BYTE _74;						//0x1E8
	WORD wCTemp;					//0x1E9	
	WORD CharFlags;					//0x1EB
	BYTE bNoMonsters;				//0x1ED
	DWORD dwMonsterClass;			//0x1EE
	BYTE bMonsterInfo;				//0x1F2
	DWORD dwMonsterDebug;			//0x1F3
	BYTE bItemRare;					//0x1F7
	BYTE bItemUnique;				//0x1F8
	BYTE _76[2];					//0x1F9
	DWORD dwAct;					//0x1FB -act default = 1
	BYTE bNoPreload;				//0x1FF -npl
	BYTE bDirect;					//0x200 -direct
	BYTE bLowEnd;					//0x201 -lem
	BYTE bNoGFXCompression;			//0x202 -nocompress
	DWORD dwArena;					//0x203 -> points to Arena* ?
	BYTE _76c[6];					//0x207
	BOOL(__stdcall* pfn77)();		//0x20D Called when MPQs are loaded	(Win.10172), pointed func always returns 1, mby used in older version
	BYTE bTxt;						//0x211 -txt
	BYTE bLog;						//0x212
	BYTE bMsgLog;					//0x213
	BYTE bSafeMode;					//0x214
	BYTE bNoSave;					//0x215
	DWORD dwSeed;					//0x216
	BYTE bCheats;					//0x21A
	BYTE bTeen;						//0x21B
	BYTE bNoSound;					//0x21C -ns
	BYTE bQuests;					//0x21D -questall
	BYTE _80;						//0x21E
	BYTE bBuild;					//0x21F -build
	BYTE bSoundBackground;			//0x220 -sndbkg
	DWORD* pfnBnetCallbacks;		//0x221 -comint
	BYTE _81[28];					//0x225
	char szGamePass[24];			//0x241
	DWORD _83[64];					//0x259
	BYTE bSkipToBnet;				//0x359 -skiptobnet
	DWORD _84[27];					//0x35A
	BYTE _85[2];					//0x3C6
};
#pragma pack(pop)

struct GfxCell  // Size is 0x2C <- to be verified
{
	DWORD flags;					//0x00
	DWORD width;					//0x04
	DWORD height;					//0x08
	int xoffs;						//0x0C
	int yoffs;						//0x10
	DWORD _2;						//0x14
	DWORD lpParent;					//0x18
	DWORD length;					//0x1C
	BYTE cols;						//0x20
};


/* To Find
	BYTE nComponents;				//0x1D
	struct
	{
	DWORD dwUnitToken;			//0x30
	DWORD dwCompToken;			//0x34
	DWORD dwSomeToken;			//0x38
	DWORD _3;					//0x3C
	DWORD dwWClassToken;		//0x40
	} tUnitInfo;
	*/
struct CellContext		//size 0x48
{
	DWORD nCellNo;					//0x00
	DWORD _0a;						//0x04
	DWORD dwUnit;					//0x08
	DWORD dwClass;					//0x0C
	DWORD dwMode;					//0x10
	DWORD _3;						//0x14
	DWORD _4;						//0x18
	BYTE _5;						//0x1C
	BYTE _5a;						//0x1D
	WORD _6;						//0x1E
	DWORD _7;						//0x20
	DWORD _8;						//0x24
	DWORD _9;						//0x28
	char* szName;					//0x2C
	DWORD _11;						//0x30
	CellFile* pCellFile;			//0x34 also pCellInit
	DWORD _12;						//0x38
	GfxCell* pGfxCells;				//0x3C
	DWORD direction;				//0x40
	DWORD _14;						//0x44
};


struct RosterInfo
{
	DWORD dwUnitId;
	DWORD dwPartyFlags;  //0x02 - ignore, 0x04 - squelch, 0x08 hostile
	RosterInfo* ptNext;
};

struct RosterPet //size 0x34
{
	DWORD dwMonStatsIdx;//0x00
	DWORD dwPetTypeIdx;	//0x04
	DWORD dwUnitId;		//0x08
	DWORD dwOwnerId;	//0x0C
	DWORD _4;			//0x10
	DWORD _5;			//0x14
	DWORD _6;			//0x18
	DWORD LifePercent;	//0x1C
	DWORD bDrawIcon;	//0x20
	void* _8;			//0x24
	DWORD _9;			//0x28
	DWORD _10;			//0x2C
	RosterPet* ptNext;	//0x30
};

struct RosterBody
{
	DWORD CorpseId;
	RosterBody* ptPrevCorpse;
};

struct RosterUnit {
	char szName[16];				//0x00
	DWORD dwUnitId;					//0x10
	DWORD dwPartyLife;				//0x14
	DWORD KillCount;				//0x18
	DWORD dwClassId;                //0x1C
	WORD wLevel;					//0x20
	WORD wPartyId;  				//0x22
	DWORD dwLevelId;                //0x24
	DWORD Xpos;						//0x28
	DWORD Ypos;						//0x2C
	DWORD dwPartyFlags;				//0x30
	RosterInfo** ptInfo;			//0x34
	RosterBody* ptBody;				//0x38
	DWORD _6[10];					//0x3C
	WORD _7;						//0x64
	char szName2[16];				//0x66
	WORD _8;						//0x76
	DWORD _9[2];					//0x78
	RosterUnit* pNext;				//0x80
};
#endif