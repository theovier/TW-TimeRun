#ifndef FLAGBOTSPAWN_H
#define FLAGBOTSPAWN_H

#include "botspawn.h"
#include <game/server/gamecontext.h>

class CFlagBotSpawn : public CBotSpawn {

public:
	CFlagBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerEXP *pController);
	virtual void OnSpawnlingsDeath() {};
};

#endif
