#ifndef GRENADEBOTSPAWN_H
#define GRENADEBOTSPAWN_H

#include <game/server/gamecontext.h>

class CGrenadeBotSpawn : public CBotSpawn {

public:
	CGrenadeBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerTimeRun *pController);
};

#endif
