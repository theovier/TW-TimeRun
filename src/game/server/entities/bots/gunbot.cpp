#include <game/server/gamecontext.h>
#include "gunbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CGunbot, MAX_CLIENTS)

CGunbot::CGunbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_GunBotLife;;
	m_Skin = "brownbear";
}

void CGunbot::SelectAppropriateWeapon(float distanceToTarget) {
	if (distanceToTarget < m_MeeleRange)
		m_QueuedWeapon = WEAPON_HAMMER;
	else
		m_QueuedWeapon = WEAPON_GUN;
}

void CGunbot::Fire(vec2 Target) {
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

