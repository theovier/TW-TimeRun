#include <game/server/gamecontext.h>
#include "gunbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CGunbot, MAX_CLIENTS)

CGunbot::CGunbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_GunBotLife;
	GiveWeapon(WEAPON_GUN, -1);
	m_Range = m_GunRange;
	m_Skin = "brownbear";
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
	bool rng = frandom() > 0.35f;
	if (hasLOS && inRange && rng) {
		return;
	}

	bool Jumping = (bool)m_Input.m_Jump;
	if (m_Pos.x < Target.x)
		m_Input.m_Direction = 1;
	else if (m_Pos.x > Target.x)
		m_Input.m_Direction = -1;

	vec2 FuturePos = vec2(m_Pos.x + m_Input.m_Direction * 100, m_Pos.y);

	if ((GameServer()->Collision()->IntersectLine(m_Pos, FuturePos, NULL, NULL, false) || m_Pos.y > Target.y) && !Jumping)
	{
		if (IsGrounded() || m_Core.m_Vel.y > -0.3f)
			m_Input.m_Jump = 1;
	}
}

