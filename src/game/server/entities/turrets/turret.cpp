#include <game/server/gamecontext.h>
#include <game/server/entities/projectile.h>
#include <game/server/botplayer.h>
#include "turret.h"

CTurret::CTurret(CGameWorld *pGameWorld, vec2 Pos) : CTrap(pGameWorld, Pos) {
	m_Pos = Pos;
	m_Alive = true;
	m_DamageTaken = 0;
}

void CTurret::TakeDamage(int Damage, int From) {
	//todo
}

void CTurret::Die(int Killer) {
	//todo
}

void CTurret::TryRespawn() {
	//todo
}

void CTurret::Respawn() {
	//todo
}



