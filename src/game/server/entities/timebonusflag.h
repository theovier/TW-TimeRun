#ifndef GAME_SERVER_ENTITIES_TIMEBONUS_H
#define GAME_SERVER_ENTITIES_TIMEBONUS_H

#include <game/server/entity.h>

class CTimeBonusFlag : public CEntity {

public:
	CTimeBonusFlag(CGameWorld *pGameWorld, vec2 Pos);

	virtual void Tick();
	virtual void Reset();
	virtual void Snap(int SnappingClient);

private:
	bool m_Collected;
	int m_TimeGain;
};

#endif