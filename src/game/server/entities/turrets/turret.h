#ifndef GAME_SERVER_ENTITIES_TURRET_H
#define GAME_SERVER_ENTITIES_TURRET_H

#include <game/server/gamecontext.h>

class CTurret : public CTrap {

public:
	CTurret(CGameWorld *pWorld, vec2 Pos);

	bool IsAlive() const { return m_Alive; }
	virtual void TakeDamage(int Damage, int From);

protected:
	int m_Health;
	bool m_Alive;
	int m_DamageTakenTick;
	int m_DamageTaken;
	int m_RespawnTick;
	
	
	virtual void Die(int Killer);
	virtual void TryRespawn();
	virtual void Respawn();
};
#endif