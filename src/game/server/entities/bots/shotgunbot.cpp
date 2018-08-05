#include <game/server/gamecontext.h>
#include "shotgunbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CShotgunBot, MAX_CLIENTS)

CShotgunBot::CShotgunBot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_ShotgunBotLife;
	GiveWeapon(WEAPON_SHOTGUN, 15);
	m_Skin = "redbopp";
}

void CShotgunBot::SelectAppropriateWeapon(float distanceToTarget) {
	if (distanceToTarget < m_MeeleRange)
		m_QueuedWeapon = WEAPON_HAMMER;
	else if (distanceToTarget < m_ShotgunRange && m_aWeapons[WEAPON_SHOTGUN].m_Ammo > 0)
		m_QueuedWeapon = WEAPON_SHOTGUN;
	else
		m_QueuedWeapon = WEAPON_GUN;
}

void CShotgunBot::Fire(vec2 Target) {
	CBotCharacter::Fire(Target);
	bool HasLineOfSight = !GameServer()->Collision()->IntersectLine(m_Pos, Target, NULL, NULL, false);
	if (HasLineOfSight) {
		bool InRange = distance(Target, m_Pos) < m_GunRange;
		bool AppropriateWeaponActive = m_QueuedWeapon == m_ActiveWeapon;
		bool HasAmmo = m_aWeapons[m_ActiveWeapon].m_Ammo != 0;
		bool FinishedReloading = m_ReloadTimer == 0;
		bool ReadyToAttack = Server()->Tick() - m_AttackTimer > 0;
		bool CanFire = InRange && AppropriateWeaponActive && HasAmmo && FinishedReloading && ReadyToAttack;

		if (CanFire) {
			m_LatestInput.m_Fire = 1;
			m_Input.m_Fire = 1;
			m_AttackTimer = Server()->Tick() + 0.4f * Server()->TickSpeed();
		}
	}
}

