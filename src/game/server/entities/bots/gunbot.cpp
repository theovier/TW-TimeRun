#include <game/server/gamecontext.h>
#include "gunbot.h"

MACRO_ALLOC_POOL_ID_IMPL(CGunbot, MAX_CLIENTS)

CGunbot::CGunbot(CGameWorld *pWorld) : CBotCharacter(pWorld) {
	CBotCharacter::CBotCharacter(pWorld);
	m_MaxHealth = m_Health = GameServer()->Tuning()->m_GunBotLife;
	//m_AttackSpeed = 2.0f;
	GiveWeapon(WEAPON_GUN, -1);
	m_Range = m_GunRange;
	m_Skin = "brownbear";
}

void CGunbot::SelectAppropriateWeapon(float distanceToTarget) {
	if (distanceToTarget > m_MeeleRange && m_aWeapons[WEAPON_GUN].m_Ammo > 0)
		m_QueuedWeapon = WEAPON_GUN;
	else
		m_QueuedWeapon = WEAPON_HAMMER;
}

