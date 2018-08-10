#ifndef GAME_SERVER_ENTITIES_MOVINGBOSSPICKUP_H
#define GAME_SERVER_ENTITIES_MOVINGBOSSPICKUP_H

#include <game/server/entity.h>
#include "movingpickup.h"

class CMovingBossPickup : public CMovingPickup {

public:
	CMovingBossPickup(CGameWorld *pGameWorld, vec2 From, CEntity *Target, float Duration, int Type, int SubType = 0);
	virtual void Tick() override;

protected:
	virtual void TickPickup() override;

private:
	CEntity* m_Target;
};
#endif
