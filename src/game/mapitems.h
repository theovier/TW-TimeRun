 /* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_MAPITEMS_H
#define GAME_MAPITEMS_H

// layer types
enum
{
	LAYERTYPE_INVALID = 0,
	LAYERTYPE_GAME,
	LAYERTYPE_TILES,
	LAYERTYPE_QUADS,

	MAPITEMTYPE_VERSION = 0,
	MAPITEMTYPE_INFO,
	MAPITEMTYPE_IMAGE,
	MAPITEMTYPE_ENVELOPE,
	MAPITEMTYPE_GROUP,
	MAPITEMTYPE_LAYER,
	MAPITEMTYPE_ENVPOINTS,


	CURVETYPE_STEP = 0,
	CURVETYPE_LINEAR,
	CURVETYPE_SLOW,
	CURVETYPE_FAST,
	CURVETYPE_SMOOTH,
	NUM_CURVETYPES,

	// game layer tiles
	ENTITY_NULL = 0,

	ENTITY_DOOR_1,
	ENTITY_DOOR_2,
	ENTITY_DOOR_3,
	ENTITY_DOOR_4,
	ENTITY_DOOR_5,
	ENTITY_DOOR_6,
	ENTITY_DOOR_7,
	ENTITY_DOOR_8,
	ENTITY_DOOR_9,
	ENTITY_DOOR_10,
	ENTITY_DOOR_11,
	ENTITY_DOOR_12,
	ENTITY_DOOR_13,
	ENTITY_DOOR_14,
	ENTITY_DOOR_15,
	ENTITY_DOOR_16,
	ENTITY_DOOR_17,
	ENTITY_DOOR_18,
	ENTITY_DOOR_19,
	ENTITY_DOOR_20,
	ENTITY_DOOR_21,
	ENTITY_DOOR_22,
	ENTITY_DOOR_23,
	ENTITY_DOOR_24,
	ENTITY_DOOR_25,
	ENTITY_DOOR_26,
	ENTITY_DOOR_27,
	ENTITY_DOOR_28,
	ENTITY_DOOR_29,
	ENTITY_DOOR_30,
	ENTITY_DOOR_31,
	ENTITY_DOOR_32,

	ENTITY_TRIGGER_DOOR_1,
	ENTITY_TRIGGER_DOOR_2,
	ENTITY_TRIGGER_DOOR_3,
	ENTITY_TRIGGER_DOOR_4,
	ENTITY_TRIGGER_DOOR_5,
	ENTITY_TRIGGER_DOOR_6,
	ENTITY_TRIGGER_DOOR_7,
	ENTITY_TRIGGER_DOOR_8,
	ENTITY_TRIGGER_DOOR_9,
	ENTITY_TRIGGER_DOOR_10,
	ENTITY_TRIGGER_DOOR_11,
	ENTITY_TRIGGER_DOOR_12,
	ENTITY_TRIGGER_DOOR_13,
	ENTITY_TRIGGER_DOOR_14,
	ENTITY_TRIGGER_DOOR_15,
	ENTITY_TRIGGER_DOOR_16,
	ENTITY_TRIGGER_DOOR_17,
	ENTITY_TRIGGER_DOOR_18,
	ENTITY_TRIGGER_DOOR_19,
	ENTITY_TRIGGER_DOOR_20,
	ENTITY_TRIGGER_DOOR_21,
	ENTITY_TRIGGER_DOOR_22,
	ENTITY_TRIGGER_DOOR_23,
	ENTITY_TRIGGER_DOOR_24,
	ENTITY_TRIGGER_DOOR_25,
	ENTITY_TRIGGER_DOOR_26,
	ENTITY_TRIGGER_DOOR_27,
	ENTITY_TRIGGER_DOOR_28,
	ENTITY_TRIGGER_DOOR_29,
	ENTITY_TRIGGER_DOOR_30,
	ENTITY_TRIGGER_DOOR_31,
	ENTITY_TRIGGER_DOOR_32,


	ENTITY_SPAWN,
	ENTITY_SPAWN_RED,
	ENTITY_SPAWN_BLUE,
	ENTITY_FLAGSTAND_RED,
	ENTITY_FLAGSTAND_BLUE,
	ENTITY_ARMOR_1,
	ENTITY_HEALTH_1,
	ENTITY_WEAPON_SHOTGUN,
	ENTITY_WEAPON_GRENADE,
	ENTITY_POWERUP_NINJA,
	ENTITY_WEAPON_RIFLE,

	ENTITY_TURRET_LASER = 17 + 64,
	ENTITY_TURRET_GUN = 33 + 64,

	ENTITY_MINE = 18 + 64,
	ENTITY_TRAP_DOWN = 34 + 64,
	ENTITY_TRAP_UP = 35 + 64,
			
	ENTITY_SPAWN_BOT_HAMMER = 40 + 64,
	ENTITY_SPAWN_BOT_GUN = 41 + 64,
	ENTITY_SPAWN_BOT_NINJA = 42 + 64,
	ENTITY_SPAWN_BOT_KAMIKAZE = 43 + 64, 
	ENTITY_SPAWN_BOT_SHOTGUN = 44 + 64,

	ENTITY_SPAWN_BOT_GRENADE = 56 + 64,
	ENTITY_SPAWN_BOT_LASER = 57 + 64,
	ENTITY_SPAWN_BOT_THOR = 58 + 64,
	ENTITY_SPAWN_BOT_FLAGBEARER = 59 +64,
	ENTITY_SPAWN_BOT_ENDBOSS = 60 + 64,

	NUM_ENTITIES=64 + 64,

	TILE_AIR=0,
	TILE_SOLID,
	TILE_DEATH,
	TILE_NOHOOK,
	TILE_HEALING,
	TILE_POISON,
	TILE_WEAPONSTRIP,
	TILE_DOOR_TRIGGER_NEAREST,

	TELEPORT_OFFSET=35,
	NUM_TELEPORTS=50,

	TILEFLAG_VFLIP=1,
	TILEFLAG_HFLIP=2,
	TILEFLAG_OPAQUE=4,
	TILEFLAG_ROTATE=8,

	LAYERFLAG_DETAIL=1,
	TILESLAYERFLAG_GAME=1,

	ENTITY_OFFSET=255-16*4-16*4,
};

struct CPoint
{
	int x, y; // 22.10 fixed point
};

struct CColor
{
	int r, g, b, a;
};

struct CQuad
{
	CPoint m_aPoints[5];
	CColor m_aColors[4];
	CPoint m_aTexcoords[4];

	int m_PosEnv;
	int m_PosEnvOffset;

	int m_ColorEnv;
	int m_ColorEnvOffset;
};

class CTile
{
public:
	unsigned char m_Index;
	unsigned char m_Flags;
	unsigned char m_Skip;
	unsigned char m_Reserved;
};

struct CMapItemInfo
{
	int m_Version;
	int m_Author;
	int m_MapVersion;
	int m_Credits;
	int m_License;
} ;

struct CMapItemImage_v1
{
	int m_Version;
	int m_Width;
	int m_Height;
	int m_External;
	int m_ImageName;
	int m_ImageData;
} ;

struct CMapItemImage : public CMapItemImage_v1
{
	enum { CURRENT_VERSION=2 };
	int m_Format;
};

struct CMapItemGroup_v1
{
	int m_Version;
	int m_OffsetX;
	int m_OffsetY;
	int m_ParallaxX;
	int m_ParallaxY;

	int m_StartLayer;
	int m_NumLayers;
} ;


struct CMapItemGroup : public CMapItemGroup_v1
{
	enum { CURRENT_VERSION=3 };

	int m_UseClipping;
	int m_ClipX;
	int m_ClipY;
	int m_ClipW;
	int m_ClipH;

	int m_aName[3];
} ;

struct CMapItemLayer
{
	int m_Version;
	int m_Type;
	int m_Flags;
} ;

struct CMapItemLayerTilemap
{
	CMapItemLayer m_Layer;
	int m_Version;

	int m_Width;
	int m_Height;
	int m_Flags;

	CColor m_Color;
	int m_ColorEnv;
	int m_ColorEnvOffset;

	int m_Image;
	int m_Data;

	int m_aName[3];
} ;

struct CMapItemLayerQuads
{
	CMapItemLayer m_Layer;
	int m_Version;

	int m_NumQuads;
	int m_Data;
	int m_Image;

	int m_aName[3];
} ;

struct CMapItemVersion
{
	int m_Version;
} ;

struct CEnvPoint
{
	int m_Time; // in ms
	int m_Curvetype;
	int m_aValues[4]; // 1-4 depending on envelope (22.10 fixed point)

	bool operator<(const CEnvPoint &Other) { return m_Time < Other.m_Time; }
} ;

struct CMapItemEnvelope_v1
{
	int m_Version;
	int m_Channels;
	int m_StartPoint;
	int m_NumPoints;
	int m_aName[8];
} ;

struct CMapItemEnvelope : public CMapItemEnvelope_v1
{
	enum { CURRENT_VERSION=2 };
	int m_Synchronized;
};

#endif
