#ifndef BOSSBOTSPAWN_H
#define BOSSBOTSPAWN_H

#include <game/server/gamecontext.h>
#include <game/server/gamemodes/exp/bots.h>

class CBossBotSpawn : public CBotSpawn {

public:
	CBossBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerEXP *pController);
};

#endif
