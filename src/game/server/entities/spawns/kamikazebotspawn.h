#ifndef KAMIKAZEBOTSPAWN_H
#define KAMIKAZEBOTSPAWN_H

#include <game/server/gamecontext.h>

class CKamikazeBotSpawn : public CBotSpawn {

public:
	CKamikazeBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerTimeRun *pController);
};

#endif
