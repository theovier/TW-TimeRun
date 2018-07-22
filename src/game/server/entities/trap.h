#ifndef GAME_SERVER_ENTITIES_TRAP_H
#define GAME_SERVER_ENTITIES_TRAP_H

#include <game/server/gamecontext.h>

class CTrap : public CEntity {

public:
	CTrap(CGameWorld *pWorld, vec2 Pos);

	virtual void Tick();

protected:
	vec2 m_Offset = vec2(0, -14.0f);
	vec2 m_Dir = vec2(0, 1);
	bool m_Active;
	bool m_IgnoreBots = true;
	float m_FireTick;
	float m_Range = 600.0f;
	int m_Damage = 5;
	bool m_Explosive = true;

	virtual void Arm();
	virtual bool Triggered(CCharacter* victim);
	virtual bool VictimInRange(vec2 victimPos);
	virtual void Fire();
};
#endif