#include <game/server/gamecontext.h>
#include <game/server/entities/laser.h>
#include "laserturret.h"

CLaserTurret::CLaserTurret(CGameWorld *pGameWorld, vec2 Pos) : CTurret(pGameWorld, Pos) {
	m_Health = 10;
	m_MaxHealth = 10;
	m_RespawnTime = 15;
}

void CLaserTurret::Fire(CCharacter* at) {
	vec2 Direction = normalize(at->GetPos() - m_Pos);
	new CLaser(&GameServer()->m_World,m_Pos, Direction, GameServer()->Tuning()->m_TurretLaserRadius, -1, true, false);
	GameServer()->CreateSound(m_Pos, SOUND_RIFLE_FIRE);
	m_FireTick = Server()->Tick() + Server()->TickSpeed() * GameServer()->Tuning()->m_TurretReload;
}