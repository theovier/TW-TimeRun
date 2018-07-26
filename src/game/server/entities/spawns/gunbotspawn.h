#ifndef GUNBOTSPAWN_H
#define GUNBOTSPAWN_H

#include <game/server/gamecontext.h>
#include <game/server/gamemodes/exp/bots.h>

class CGunBotSpawn : public CBotSpawn {

public:
	CGunBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerEXP *pController);
};

#endif
