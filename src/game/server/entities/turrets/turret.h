#ifndef GAME_SERVER_ENTITIES_TURRET_H
#define GAME_SERVER_ENTITIES_TURRET_H

#include <game/server/gamecontext.h>

class CTurret : public CTrap {

public:
	CTurret(CGameWorld *pWorld, vec2 Pos);

	bool IsAlive() const { return m_Active; }
	virtual void Tick() override;
	virtual void Reset() override;
	virtual void TakeDamage(int Damage, int From);

protected:
	int m_Health;
	int m_MaxHealth;
	int m_DamageTakenTick;
	int m_DamageTaken;
	int m_RespawnTick = -1;
	int m_RespawnTime = 5;
	
	
	virtual void Die(int Killer);
	virtual void Respawn();
};
#endif