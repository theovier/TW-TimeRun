#ifndef GAME_SERVER_ENTITIES_SPINTRAP_H
#define GAME_SERVER_ENTITIES_SPINTRAP_H

#include <game/server/gamecontext.h>

class CSpinTrap : public CTrap {

public:
	CSpinTrap(CGameWorld *pWorld, vec2 Pos);
	virtual void Tick();

protected:
	virtual bool VictimInRange(vec2 victimPos);
	virtual void Spin();
	float m_Angle;
};
#endif