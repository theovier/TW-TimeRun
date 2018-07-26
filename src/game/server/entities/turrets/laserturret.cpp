#include <game/server/gamecontext.h>
#include <game/server/entities/laser.h>
#include "laserturret.h"

CLaserTurret::CLaserTurret(CGameWorld *pGameWorld, vec2 Pos) : CTurret(pGameWorld, Pos) {
	m_MaxHealth = GameServer()->Tuning()->m_TurretLaserLife;
	m_Health = m_MaxHealth;
	m_RespawnTime = GameServer()->Tuning()->m_TurretLaserRespawnTime;
}

void CLaserTurret::Reset() {
	CTurret::Reset();
	m_Health = m_MaxHealth;
}

void CLaserTurret::Fire(CCharacter* at) {
	vec2 Direction = normalize(at->GetPos() - m_Pos);
	new CLaser(&GameServer()->m_World,m_Pos, Direction, GameServer()->Tuning()->m_TurretLaserReach, -1, true, false);
	GameServer()->CreateSound(m_Pos, SOUND_RIFLE_FIRE);
	m_FireTick = Server()->Tick() + Server()->TickSpeed() * GameServer()->Tuning()->m_TurretLaserAttackSpeed;
}