#include "hammerbot.h"


CHammerbot::CHammerbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_HammerBotLife;
	m_aWeapons[WEAPON_GUN].m_Got = false;
}

void CHammerbot::Fire(vec2 Target) {
	CBotCharacter::Fire(Target);
	bool HasLineOfSight = !GameServer()->Collision()->IntersectLine(m_Pos, Target, NULL, NULL, false);
	if (HasLineOfSight) {
		bool InRange = distance(Target, m_Pos) < m_MeeleRange;
		bool FinishedReloading = m_ReloadTimer == 0;
		bool ReadyToAttack = Server()->Tick() - m_AttackTimer > 0;
		bool CanFire = InRange && FinishedReloading && ReadyToAttack;

		if (CanFire) {
			m_LatestInput.m_Fire = 1;
			m_Input.m_Fire = 1;
			m_AttackTimer = Server()->Tick() + 0.4f * Server()->TickSpeed();
		}
	}
}


