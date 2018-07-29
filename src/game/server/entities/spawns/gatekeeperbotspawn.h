#ifndef GATEKEEPERBOTSPAWN_H
#define GATEKEEPERBOTSPAWN_H

#include "botspawn.h"
#include <game/server/gamecontext.h>

class CGatekeeperBotSpawn : public CBotSpawn {

public:
	CGatekeeperBotSpawn(CGameWorld *pWorld, vec2 Pos, CGameControllerTimeRun *pController, int DoorIndex);
	virtual void OnSpawnlingsDeath() {};
protected:
	virtual int Spawn() override;
private:
	int m_DoorIndex;
};

#endif
