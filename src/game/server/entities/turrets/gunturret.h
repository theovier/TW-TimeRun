#ifndef GAME_SERVER_ENTITIES_GUNTURRET_H
#define GAME_SERVER_ENTITIES_GUNTURRET_H

#include "turret.h"
#include <game/server/gamecontext.h>

class CGunTurret : public CTurret {

public:
	CGunTurret(CGameWorld *pWorld, vec2 Pos);

protected:
	virtual bool VictimInVerticalRange(vec2 victimPos) { return true; }
	virtual bool VictimInHorizontalRange(vec2 victimPos) { return true; }
	virtual void Fire(CCharacter* at);
};
#endif