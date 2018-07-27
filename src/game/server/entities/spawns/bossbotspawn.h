#ifndef BOSSBOTSPAWN_H
#define BOSSBOTSPAWN_H

#include <game/server/gamecontext.h>

class CBossBotSpawn : public CBotSpawn {

public:
	CBossBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerEXP *pController);
};

#endif
