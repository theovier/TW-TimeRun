#ifndef HAMMERBOTSPAWN_H
#define HAMMERBOTSPAWN_H

#include <game/server/gamecontext.h>

class CHammerBotSpawn : public CBotSpawn {

public:
	CHammerBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerEXP *pController);
};

#endif
