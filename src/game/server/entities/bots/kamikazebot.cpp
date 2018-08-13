#include "kamikazebot.h"

MACRO_ALLOC_POOL_ID_IMPL(CKamikazebot, MAX_CLIENTS)

CKamikazebot::CKamikazebot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_KamikazeBotLife;
	m_TriggerRange = GameServer()->Tuning()->m_KamikazeBotTriggerRadius;
	m_ExplosionDelay = GameServer()->Tuning()->m_KamikazeBotExplosionDelay;
	m_ExplosionTick = -1;
	GiveWeapon(WEAPON_KAMIKAZE, -1);
}

void CKamikazebot::Tick() {
	CBotCharacter::Tick();
	m_AttackTick = Server()->Tick();
	if (Server()->Tick() > m_ExplosionTick && m_ExplosionTick != -1) {
		Explode();
	}
}

void CKamikazebot::SelectAppropriateWeapon(float distanceToTarget) {
	SetWeapon(WEAPON_KAMIKAZE);
}

void CKamikazebot::Fire(vec2 Target) {
	StopFire();
	bool HasLineOfSight = !GameServer()->Collision()->IntersectLine(m_Pos, Target, NULL, NULL, false);
	if (HasLineOfSight) {
		bool InRange = distance(Target, m_Pos) < m_TriggerRange;
		bool CanFire = InRange && m_ExplosionTick == -1;
		if (CanFire) {
			SetEmoticon(EMOTICON_DEVILTEE);
			m_ExplosionTick = Server()->Tick() + Server()->TickSpeed() * m_ExplosionDelay;
		}
	}
}

void CKamikazebot::Explode() {
	m_LatestInput.m_Fire = 1;
	m_Input.m_Fire = 1;
}


