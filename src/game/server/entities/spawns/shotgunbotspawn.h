#ifndef SHOTGUNBOTSPAWN_H
#define SHOTGUNBOTSPAWN_H

#include <game/server/gamecontext.h>

class CShotgunBotSpawn : public CBotSpawn {

public:
	CShotgunBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerTimeRun *pController);
};

#endif
