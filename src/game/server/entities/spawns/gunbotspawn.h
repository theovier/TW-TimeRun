#ifndef GUNBOTSPAWN_H
#define GUNBOTSPAWN_H

#include <game/server/gamecontext.h>

class CGunBotSpawn : public CBotSpawn {

public:
	CGunBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerTimeRun *pController);
};

#endif
