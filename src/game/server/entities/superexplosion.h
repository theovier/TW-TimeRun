#ifndef GAME_SERVER_ENTITIES_SUPEREXPLOSION_H
#define GAME_SERVER_ENTITIES_SUPEREXPLOSION_H

#include <game/server/entity.h>

class CSuperexplosion : public CEntity {

public:
	static const int ms_PhysSize = 24;

	CSuperexplosion(CGameWorld *pGameWorld, vec2 Pos, int Player, int Weapon, int MaxLife, int StartLife = 0);

	virtual void Tick();

private:
	int m_Life;
	int m_MaxLife;
	int m_Player;
	int m_Weapon;
	int m_NextIn;
};

#endif