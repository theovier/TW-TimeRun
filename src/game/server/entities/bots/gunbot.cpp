#include <game/server/gamecontext.h>
#include "gunbot.h"

CGunbot::CGunbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_Health = 10;
}

void CGunbot::Handle()  {

	bool Jumping = (bool)m_Input.m_Jump;
	m_Input.m_Direction = 0;
	m_Input.m_Jump = 0;
	m_LatestInput.m_Fire = 0;
	m_Input.m_Fire = 0;
	
	CCharacter * TargetChr = GameServer()->m_World.ClosestCharacter(m_Pos, 700.0f, NULL);
	if(!TargetChr)
		return;


	if (distance(TargetChr->m_Pos, m_Pos) < 64.0f)
		m_QueuedWeapon = WEAPON_HAMMER;
	else
		m_QueuedWeapon = WEAPON_GUN;

	if (m_Pos.x < TargetChr->m_Pos.x)
		m_Input.m_Direction = 1;
	else if (m_Pos.x > TargetChr->m_Pos.x)
		m_Input.m_Direction = -1;

	vec2 FuturePos;
	FuturePos.x = m_Pos.x + m_Input.m_Direction * 100;
	FuturePos.y = m_Pos.y;

	if ((GameServer()->Collision()->IntersectLine(m_Pos, FuturePos, NULL, NULL, false) || m_Pos.y > TargetChr->m_Pos.y) && !Jumping)
	{
		if (IsGrounded() || m_Core.m_Vel.y > -0.3f)
			m_Input.m_Jump = 1;
	}

	Aim(TargetChr->m_Pos);

	if (!GameServer()->Collision()->IntersectLine(m_Pos, TargetChr->m_Pos, NULL, NULL, false))
	{
		if (m_QueuedWeapon != m_ActiveWeapon)
			return;
		if (distance(TargetChr->m_Pos, m_Pos) < 500.0f
			&& m_aWeapons[m_ActiveWeapon].m_Ammo != 0 && m_ReloadTimer == 0
			&& Server()->Tick() - m_AttackTimer > 0)
		{
			m_LatestInput.m_Fire = 1;
			m_Input.m_Fire = 1;
			m_AttackTimer = Server()->Tick() + 0.4f*Server()->TickSpeed();
		}
	}
	
}

