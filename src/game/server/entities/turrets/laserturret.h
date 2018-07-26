#ifndef GAME_SERVER_ENTITIES_LASERTURRET_H
#define GAME_SERVER_ENTITIES_LASERTURRET_H

#include "turret.h"
#include <game/server/gamecontext.h>

class CLaserTurret : public CTurret {

public:
	CLaserTurret(CGameWorld *pWorld, vec2 Pos);
	virtual void Reset() override;

protected:


	virtual bool VictimInVerticalRange(vec2 victimPos) { return true; }
	virtual bool VictimInHorizontalRange(vec2 victimPos) { return true; }
	virtual void Fire(CCharacter* at);
};
#endif