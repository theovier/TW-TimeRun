#ifndef GAME_SERVER_ENTITIES_TRAP_H
#define GAME_SERVER_ENTITIES_TRAP_H

#include <game/server/gamecontext.h>

class CTrap : public CEntity {

public:
	CTrap(CGameWorld *pWorld, vec2 Pos);

	virtual void Tick();

protected:
	vec2 m_Offset = vec2(0, -14.0f);
	bool m_Active;
	float m_AttackSpeed;
	float m_Range = 600.0f;
};
#endif