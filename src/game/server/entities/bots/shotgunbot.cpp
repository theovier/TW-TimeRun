#include <game/server/gamecontext.h>
#include "shotgunbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CShotgunBot, MAX_CLIENTS)

CShotgunBot::CShotgunBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_ShotgunBotLife;
	GiveWeapon(WEAPON_SHOTGUN, m_MaxShotgunAmmo);
	m_AttackSpeed = 1.6f;
	m_Range = m_ShotgunRange;
	m_HookDuration = 0.6f;
	m_HookInterval = 5.0f;
	m_Skin = "redbopp";
}

void CShotgunBot::Handle() {
	CBotCharacter::Handle();
}

void CShotgunBot::SelectAppropriateWeapon(float distanceToTarget) {
	if (distanceToTarget < m_ShotgunRange)
		m_QueuedWeapon = WEAPON_SHOTGUN;
}

void CShotgunBot::Aim(vec2 Target) {
	CBotCharacter::Aim(Target);
}

void CShotgunBot::StopReload() {
	CBotCharacter::StopReload();
	m_aWeapons[m_ActiveWeapon].m_Ammo = m_MaxShotgunAmmo;
}

