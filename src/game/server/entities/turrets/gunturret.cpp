#include <game/server/gamecontext.h>
#include <game/server/entities/projectile.h>
#include <game/server/botplayer.h>
#include "gunturret.h"

CGunTurret::CGunTurret(CGameWorld *pGameWorld, vec2 Pos) : CTurret(pGameWorld, Pos) {
	m_MaxHealth = GameServer()->Tuning()->m_TurretGunLife;
	m_Health = m_MaxHealth;
	m_RespawnTime = GameServer()->Tuning()->m_TurretGunRespawnTime;
}

void CGunTurret::Reset() {
	CTurret::Reset();
	m_Health = m_MaxHealth;
}

void CGunTurret::Fire(CCharacter* at) {
	vec2 Direction = normalize(at->GetPos() - m_Pos);
	new CProjectile(&GameServer()->m_World, WEAPON_GUN, -1, m_Pos, Direction, (int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GunLifetime), 1, 0, 0, -1, WEAPON_GUN);
	GameServer()->CreateSound(m_Pos, SOUND_GUN_FIRE);
	m_FireTick = Server()->Tick() + Server()->TickSpeed() * GameServer()->Tuning()->m_TurretGunAttackSpeed;
}