#include <game/server/gamecontext.h>
#include "gunbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CGunbot, MAX_CLIENTS)

CGunbot::CGunbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_GunBotLife;
	GiveWeapon(WEAPON_GUN, -1);
	m_Range = m_GunRange;
	m_Skin = "brownbear";
	m_DropChance = 0.8f;
}

void CGunbot::SelectAppropriateWeapon(float distanceToTarget) {
	if (distanceToTarget < m_MeeleRange && (Server()->Tick() - m_AttackTimer > 0 || m_ActiveWeapon == WEAPON_HAMMER)) {
		SetWeapon(WEAPON_HAMMER);
		m_AttackSpeed = 1.0f;
	}
	else {
		SetWeapon(WEAPON_GUN);
		m_AttackSpeed = 0.5f;
	}		
}

void CGunbot::Move(vec2 Target) {
	//todo: Causes lag because of movement prediction?

	m_Input.m_Direction = 0;
	m_Input.m_Jump = 0;

	//stand still if we have LOS + range. But just now always.
	bool hasLOS = !GameServer()->Collision()->IntersectLine(Target, m_Pos, NULL, NULL);
	bool inRange = distance(Target, m_Pos) < m_Range;
	if (hasLOS && inRange) {
		if (Server()->Tick() > m_StillstandTick) {
			m_StandStill = frandom() < m_StillstandChance;
			m_StillstandTick = Server()->Tick() + Server()->TickSpeed() * m_StillstandTime;
		}
		if (m_StandStill) {
			return;
		}
	}
	CBotCharacter::Move(Target);
}

