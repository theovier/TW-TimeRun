#include <game/server/gamecontext.h>
#include "gunbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CGunbot, MAX_CLIENTS)

CGunbot::CGunbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = 10;
}

void CGunbot::Move(vec2 Target) {
	bool Jumping = (bool)m_Input.m_Jump;
	m_Input.m_Direction = 0;
	m_Input.m_Jump = 0;

	if (m_Pos.x < Target.x)
		m_Input.m_Direction = 1;
	else if (m_Pos.x > Target.x)
		m_Input.m_Direction = -1;

	vec2 FuturePos;
	FuturePos.x = m_Pos.x + m_Input.m_Direction * 100;
	FuturePos.y = m_Pos.y;

	if ((GameServer()->Collision()->IntersectLine(m_Pos, FuturePos, NULL, NULL, false) || m_Pos.y > Target.y) && !Jumping)
	{
		if (IsGrounded() || m_Core.m_Vel.y > -0.3f)
			m_Input.m_Jump = 1;
	}
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

