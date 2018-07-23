#include <game/server/gamecontext.h>
#include <game/server/entities/projectile.h>
#include <game/server/botplayer.h>
#include "gunturret.h"

CGunTurret::CGunTurret(CGameWorld *pGameWorld, vec2 Pos) : CTurret(pGameWorld, Pos) {
	m_Health = 10;
}

void CGunTurret::Fire(CCharacter* at) {
	vec2 Direction = normalize(at->GetPos() - m_Pos);
	new CProjectile(&GameServer()->m_World, WEAPON_GUN, -1, m_Pos, Direction, (int)(Server()->TickSpeed()*GameServer()->Tuning()->m_GunLifetime), 1, 0, 0, -1, WEAPON_GUN);
	GameServer()->CreateSound(m_Pos, SOUND_GUN_FIRE);
	m_FireTick = Server()->Tick() + GameServer()->Tuning()->m_TurretReload*Server()->TickSpeed();
}